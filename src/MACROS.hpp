/* Definitions for keydown events */
#pragma once

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

/* Key macros */
#define KEYCODEA sf::Keyboard::A
#define KEYCODEB sf::Keyboard::B
#define KEYCODEC sf::Keyboard::C
#define KEYCODED sf::Keyboard::D
#define KEYCODEE sf::Keyboard::E
#define KEYCODEF sf::Keyboard::F
#define KEYCODEG sf::Keyboard::G
#define KEYCODEH sf::Keyboard::H
#define KEYCODEI sf::Keyboard::I
#define KEYCODEJ sf::Keyboard::J
#define KEYCODEK sf::Keyboard::K
#define KEYCODEL sf::Keyboard::L
#define KEYCODEM sf::Keyboard::M
#define KEYCODEN sf::Keyboard::N
#define KEYCODEO sf::Keyboard::O
#define KEYCODEP sf::Keyboard::P
#define KEYCODEQ sf::Keyboard::Q
#define KEYCODER sf::Keyboard::R
#define KEYCODES sf::Keyboard::S
#define KEYCODET sf::Keyboard::T
#define KEYCODEU sf::Keyboard::U;
#define KEYCODEV sf::Keyboard::V
#define KEYCODEW sf::Keyboard::W
#define KEYCODEX sf::Keyboard::X
#define KEYCODEY sf::Keyboard::Y
#define KEYCODEZ sf::Keyboard::Z
#define KEYCODE0 sf::Keyboard::Num0
#define KEYCODE1 sf::Keyboard::Num1
#define KEYCODE2 sf::Keyboard::Num2
#define KEYCODE3 sf::Keyboard::Num3
#define KEYCODE4 sf::Keyboard::Num4
#define KEYCODE5 sf::Keyboard::Num5
#define KEYCODE6 sf::Keyboard::Num6
#define KEYCODE7 sf::Keyboard::Num7
#define KEYCODE8 sf::Keyboard::Num8
#define KEYCODE9 sf::Keyboard::Num9
#define KEYCODEUPARROW sf::Keyboard::Up
#define KEYCODEDOWNARROW sf::Keyboard::Down
#define KEYCODELEFTARROW sf::Keyboard::Left
#define KEYCODERIGHTARROW sf::Keyboard::Right
#define KEYCODELSHIFT sf::Keyboard::LShift
#define KEYCODERSHIFT sf::Keyboard::RShift
#define KEYCODELCNTRL sf::Keyboard::LControl
#define KEYCODERCNTRL sf::Keyboard::RControl
#define KEYCODEF1 sf::Keyboard::F1
#define KEYCODEF2 sf::Keyboard::F2
#define KEYCODEF3 sf::Keyboard::F3
#define KEYCODEF4 sf::Keyboard::F4
#define KEYCODEF5 sf::Keyboard::F5
#define KEYCODEF6 sf::Keyboard::F6
#define KEYCODEF7 sf::Keyboard::F7
#define KEYCODEF8 sf::Keyboard::F8
#define KEYCODEF9 sf::Keyboard::F9
#define KEYCODEF10 sf::Keyboard::F10
#define KEYCODEF11 sf::Keyboard::F11
#define KEYCODEF12 sf::Keyboard::F12
#define KEYCODEESC sf::Keyboard::Escape
#define KEYCODESPACE sf::Keyboard::Space

/* Color macros */
#define C_TRANS sf::Color(0,0,0,0)
#define C_BLACK sf::Color(0,0,0)
#define C_DGRAY1 sf::Color(50,50,50)
#define C_DGRAY2 sf::Color(100,100,100)
#define C_LGRAY1 sf::Color(150,150,150)
#define C_LGRAY2 sf::Color(200,200,200)
#define C_WHITE sf::Color(255,255,255)
#define C_GREEN sf::Color(0,255,0)
#define C_BLUE sf::Color(0,0,255)
#define C_RED sf::Color(255,0,0)
#define C_PINK sf::Color(175,0,0)
#define C_LPINK sf::Color(100,0,0)
#define C_LBLUE sf::Color(0,0,100)
#define C_LGREEN sf::Color(0,100,0)

/* Text output colors */
#define LOG_BLACK     "\x1b[30m"
#define LOG_RED       "\x1b[31m" // Failure
#define LOG_GREEN     "\x1b[32m" // Success
#define LOG_YELLOW    "\x1b[33m" // Warning
#define LOG_BLUE      "\x1b[34m"
#define LOG_MAGENTA   "\x1b[35m"
#define LOG_CYAN      "\x1b[36m" // Internal variables
#define LOG_RESET     "\x1b[37m"