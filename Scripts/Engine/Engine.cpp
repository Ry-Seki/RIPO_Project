#include "Engine.h"
#include "Task.h"
#include <iostream>

Engine::Engine() {}
Engine::~Engine() { Teardown(); }

int Engine::Initialize() {
    return 0;
}

void Engine::Teardown() {
}

void Engine::Update() {
}

void Engine::Render() {
}

int Engine::Run() {
    return 0;
}