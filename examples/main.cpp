//
// Created by jarboola on 2/10/26.
//

import Mango;

int main() {

    Mango::Initialize("Example",300,300);
    Mango::MainLoop([] {
       Mango::DrawCircle(150,150,10,Mango::White);
    });

}