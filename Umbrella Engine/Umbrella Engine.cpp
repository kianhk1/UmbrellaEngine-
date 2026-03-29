#include "Umbrella Engine.h"
std::unique_ptr<API::GraphicsAPI> UE = std::make_unique<API::openglGraphicsAPI>();
int main() {
    std::cout << "Hello World!\n";
}
