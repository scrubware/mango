//
// Created by jarboola on 1/16/26.
//

module;

#include <functional>
#include <stack>
#include <variant>

export module Mango.Schedule;

/*
 *
 *  API
 *  For Schedules that are meant to be executed every frame, we offer Wrappers as well as
 *  toggleable schedule steps as the type alias Toggle.
 *
 *  For single-execution Schedules, you'll likely only use Step. This is effectively the
 *  same thing as using a vector, except each step also has an ID attached.
 *
 */

namespace Mango {
    export template <typename IDType, typename... Args> struct Schedule {

        // ID, Callable
        using Step = std::pair<IDType, std::function<void(Args...)>>;
        // Should do Callable?, ID, Callable
        using Toggle = std::tuple<bool, IDType, std::function<void(Args...)>>;
        // Should do Wrap Callable?, Should do inner Callables?, ID, Wrap Callable
        using Start = std::tuple<bool, bool, IDType, std::function<void(Args...)>>;
        // ID, End Wrap Callable
        using End = std::tuple<IDType, std::function<void(Args...)>>;

        using Member = std::variant<Step, Toggle, Start, End>;

    private:

        std::vector<Member> schedule;

    public:

        Schedule& Push(Member member) {
            schedule.push_back(member);
            return *this;
        }

        // Schedule& Place(Member member, IDType id) {
        //     // TODO: replace existing member or append.
        //     return *this;
        // }

        void Process(Args... args) const {
            std::stack<std::pair<Start, bool>> scope_stack;

            for (auto& member : schedule)
            {
                // Don't be alarmed by this loop body. We're just doing all this pointer/if-init stuff because of std::variant.
                if (auto s = std::get_if<Step>(&member)) {
                    auto& [id, func] = *s;
                    auto& [ws, parent_do] = scope_stack.top();

                    if (!parent_do or !func) continue;
                    func(args...);
                } else if (auto t = std::get_if<Toggle>(&member)) {
                    auto& [draw, id, func] = *t;
                    auto& [ws, parent_do] = scope_stack.top();

                    if (!draw or !parent_do or !func) continue;
                    func(args...);

                } else if (auto ws = std::get_if<Start>(&member)) {
                    auto& [do_wrapper, do_inner, id, start_func] = *ws;

                    // The purpose of the bool in the scope_stack's pairs is to signify if inner scope should be drawn.
                    // This is separate because it's inherited from stack layer to stack layer.
                    // Here we are doing that inheritance.
                    bool parent_do = scope_stack.empty() ? do_inner : scope_stack.top().second && do_inner;

                    scope_stack.push({*ws,parent_do});

                    if (!do_wrapper or !start_func) continue;
                    start_func(args...);

                } else if (auto we = std::get_if<End>(&member)) {
                    auto& [id, end_func] = *we;
                    auto& [_ws, parent_do] = scope_stack.top();
                    auto& [do_wrapper, do_inner, start_id, start_func] = _ws;

                    if (id != start_id) throw "No Mango::Schedule<>::Start object for this id!";

                    scope_stack.pop();

                    if (!do_wrapper or !end_func) continue;
                    end_func(args...);
                }
            }
        };
    };
}
