//
// Created by tom on 12/16/2025.
//
module;

#include <any>
#include <array>
#include <iostream>
#include <memory>
#include <unordered_map>

export module Mango.Screen;

/* API
 *
 * Create Screens by inheriting Screen
 * - Screen constructor must accept a ScreenManager reference
 * - Screen constructor must pass a string key into the parent constructor
 * - Screen must implement the virtual Draw() function.
 * - Screen can optionally implement the virtual Update() function.
 *
 * For RESOURCE ALLOCATION:
 * - Screens should implement onEnter() to allocate resources before the screen is transitioned or switched to.
 * - Screens should implement onLeave() to free up resources once the screen is transitioned or switched away from.
 * - It is recommended that your onEnter() logic be versatile enough to handle being run multiple times, as this
 *   method may be utilized later for a "refresh screen state" functionality.
 *
 * For TRANSITIONS:
 * - Screens should implement onTransitionTo() if they want to implement their visual transition logic and that logic
 *   requires initializing or resetting some state.
 * - Screens should implement onTransitionFrom() if they want to implement their visual transition logic and that logic
 *   requires initializing or resetting some state.
 * - It is recommended that all transitional logic should be initialized here, and NOT in onEnter, since these methods
 *   are not called when screens are SWITCHED between (instant transitions).
 * - Transitions should access the ScreenManager's transition speed multiplier via calling manager.GetTransitionSpeed().
 * - Once your transition-to is complete, call DoneTransitioningTo(), and once your transition-from is complete, call
 *   DoneTransitioningFrom(). These are the ONLY methods you should call yourself!
 * - Screens automatically have access to the ScreenManager via `manager` and can get transition state using
 *   GetEntering() and GetLeaving()
 * - Entering/Leaving states should describe the time in which the screen is NOT USABLE. This means that you may have
 *   additional animations that play later in the screens existence, but after it becomes usable. Manage those states
 *   on your own.
 *
 *   The DoneTransitioning Functions will mark GetEntering()/GetLeaving() states as over AFTER the current frame ends.
 *
 * Usage:
 * - Instantiate a ScreenManager.
 * - Call Initialize() templated with your screen types.
 * - Call SwitchTo() on your manager to set the initial screen.
 * - Call Draw() in your draw, and Update() in your update.
 *
 */

// TODO: Update() takes a timestep.
// TODO: General lower-priority Update() system, like 5, 10, 30, and 60hz. Although those should be strictly typed as Lowest, Highest, etc.

namespace Mango {
	export struct ScreenManager;

	export using ScreenKey = std::string_view;
	export using ScreenContext = std::any;

	export struct Screen
	{
		friend ScreenManager;

	private:
		bool entering = false;
		bool leaving = false;

		ScreenManager& manager;
		ScreenKey key;

		ScreenKey GetKey() const { return key; }

	protected:

		// Children should implement:
		virtual void Draw() = 0;
		virtual void Update() {};

		virtual void onLeave() {}
		virtual void onEnter(ScreenContext context) {}
		virtual void onTransitionTo() {}
		virtual void onTransitionFrom() {}

		// Children should use:
		Screen(ScreenManager& manager, ScreenKey key);

		void DoneTransitioningFrom() const;
		void DoneTransitioningTo() const;

		bool GetEntering() const { return entering; }
		bool GetLeaving() const { return leaving; }
		ScreenManager& GetManager() const { return manager; }

	public:
		virtual ~Screen() = default;
	};

	export struct ScreenManager final
	{

		friend void Screen::DoneTransitioningFrom() const;
		friend void Screen::DoneTransitioningTo() const;

	private:
		Screen* current_screen = nullptr;
		Screen* changing_screen = nullptr;
		Screen* stale_screen = nullptr;
		Screen* fresh_screen = nullptr;
		std::unordered_map<ScreenKey, std::unique_ptr<Screen>> screens;

	public:
		void SwitchTo(const ScreenKey key, const ScreenContext& context = {})
		{
			if (!screens.contains(key))
			{
				std::cout << "Attempted to switch to screen \"" << key << "\" which is not registered.\n";
				return;
			}
			current_screen = screens.at(key).get();
			current_screen->onEnter(context);
		}

		void TransitionTo(const ScreenKey key, const ScreenContext& context = {})
		{
			if (!screens.contains(key))
			{
				std::cout << "Attempted to transition to screen \"" << key << "\" which is not registered.\n";
				return;
			}
			current_screen->leaving = true;
			changing_screen = screens.at(key).get();
			changing_screen->onEnter(context);
			current_screen->onTransitionFrom();
		}

		// We will create an overload of Register when we support screens in modding.
		template <typename T> requires std::is_base_of_v<Screen, T>
		void Initialize() {
			auto screen = std::make_unique<T>(*this);
			screens.insert(std::pair(screen->GetKey(),std::move(screen)));
		}

		void Draw()
		{
			if (stale_screen != nullptr)
			{
				stale_screen->onLeave();
				stale_screen->leaving = false;
				stale_screen = nullptr;
			}

			if (fresh_screen != nullptr)
			{
				fresh_screen->entering = false;
				fresh_screen = nullptr;
			}

			current_screen->Draw();
		}

		void Update()
		{
			current_screen->Update();
		}
	};

	// Constructor must be down here or Screen does not have a full definition of ScreenManager.
	Screen::Screen(ScreenManager& manager, const ScreenKey key): manager(manager), key(key) {}
	void Screen::DoneTransitioningFrom() const
	{
		manager.stale_screen = manager.current_screen;
		manager.current_screen = manager.changing_screen;
		manager.changing_screen = nullptr;
		manager.current_screen->onTransitionTo();
		manager.current_screen->entering = true;
	}

	void Screen::DoneTransitioningTo() const
	{
		manager.fresh_screen = manager.current_screen;
	}
}
