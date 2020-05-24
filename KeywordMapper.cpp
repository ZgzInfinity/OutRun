/******************************************************************************
 * @file    KeywordMapper.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Rubén Rodríguez Esteban, 737215
 * @date    Mayo 2020
 * @coms    Videojuegos - OutRun
 ******************************************************************************/

#include "KeywordMapper.hpp"

using namespace std;
using namespace sf;

KeywordMapper::KeywordMapper() {

    // Keyword letters
    mapperIdKeyWord[0] = "A";
    mapperIdKeyWord[1] = "B";
    mapperIdKeyWord[2] = "C";
    mapperIdKeyWord[3] = "D";
    mapperIdKeyWord[4] = "E";
    mapperIdKeyWord[5] = "F";
    mapperIdKeyWord[6] = "G";
    mapperIdKeyWord[7] = "H";
    mapperIdKeyWord[8] = "I";
    mapperIdKeyWord[9] = "J";
    mapperIdKeyWord[10] = "K";
    mapperIdKeyWord[11] = "L";
    mapperIdKeyWord[12] = "M";
    mapperIdKeyWord[13] = "N";
    mapperIdKeyWord[14] = "0";
    mapperIdKeyWord[15] = "P";
    mapperIdKeyWord[16] = "Q";
    mapperIdKeyWord[17] = "R";
    mapperIdKeyWord[18] = "S";
    mapperIdKeyWord[19] = "T";
    mapperIdKeyWord[20] = "U";
    mapperIdKeyWord[21] = "V";
    mapperIdKeyWord[22] = "W";
    mapperIdKeyWord[23] = "X";
    mapperIdKeyWord[24] = "Y";
    mapperIdKeyWord[25] = "Z";

    // Numbers
    mapperIdKeyWord[26] = "0";
    mapperIdKeyWord[27] = "1";
    mapperIdKeyWord[28] = "2";
    mapperIdKeyWord[29] = "3";
    mapperIdKeyWord[30] = "4";
    mapperIdKeyWord[31] = "5";
    mapperIdKeyWord[32] = "6";
    mapperIdKeyWord[33] = "7";
    mapperIdKeyWord[34] = "8";
    mapperIdKeyWord[35] = "9";

    // Control keywords
    mapperIdKeyWord[36] = "ESC";
    mapperIdKeyWord[37] = "LCTRL";
    mapperIdKeyWord[38] = "LSHITF";
    mapperIdKeyWord[39] = "LALT";
    mapperIdKeyWord[40] = "LSYSTEM";
    mapperIdKeyWord[41] = "RCTRL";
    mapperIdKeyWord[42] = "RSHITF";
    mapperIdKeyWord[43] = "RALT";
    mapperIdKeyWord[44] = "RSYSTEM";
    mapperIdKeyWord[45] = "MENU";
    mapperIdKeyWord[46] = "LBRACKET";
    mapperIdKeyWord[47] = "RBRACKET";
    mapperIdKeyWord[48] = "SEMICOLOM";
    mapperIdKeyWord[49] = "COMMA";
    mapperIdKeyWord[50] = "PERIOD";
    mapperIdKeyWord[51] = "QUOTE";

    mapperIdKeyWord[52] = "SLASH";
    mapperIdKeyWord[53] = "BACKLASH";
    mapperIdKeyWord[54] = "TILDE";
    mapperIdKeyWord[55] = "EQUAL";
    mapperIdKeyWord[56] = "HYPHEN";
    mapperIdKeyWord[57] = "SPACE";
    mapperIdKeyWord[58] = "ENTER";
    mapperIdKeyWord[59] = "BACKSPACE";
    mapperIdKeyWord[60] = "TAB";
    mapperIdKeyWord[61] = "REPAG";
    mapperIdKeyWord[62] = "AVPAG";
    mapperIdKeyWord[63] = "END";
    mapperIdKeyWord[64] = "HOME";
    mapperIdKeyWord[65] = "INSERT";
    mapperIdKeyWord[66] = "DELETE";
    mapperIdKeyWord[67] = "ADD";
    mapperIdKeyWord[68] = "SUB";
    mapperIdKeyWord[69] = "MUL";
    mapperIdKeyWord[70] = "DIV";
    mapperIdKeyWord[71] = "LEFT CURSOR";
    mapperIdKeyWord[72] = "RIGHT CURSOR";
    mapperIdKeyWord[73] = "UP CURSOR";
    mapperIdKeyWord[74] = "DOWN CURSOR";

    // Map the code of the keys
    mapperCodeKeyWord[0] = Keyboard::A;
    mapperCodeKeyWord[1] = Keyboard::B;
    mapperCodeKeyWord[2] = Keyboard::C;
    mapperCodeKeyWord[3] = Keyboard::D;
    mapperCodeKeyWord[4] = Keyboard::E;
    mapperCodeKeyWord[5] = Keyboard::F;
    mapperCodeKeyWord[6] = Keyboard::G;
    mapperCodeKeyWord[7] = Keyboard::H;
    mapperCodeKeyWord[8] = Keyboard::I;
    mapperCodeKeyWord[9] = Keyboard::J;
    mapperCodeKeyWord[10] = Keyboard::K;
    mapperCodeKeyWord[11] = Keyboard::L;
    mapperCodeKeyWord[12] = Keyboard::M;
    mapperCodeKeyWord[13] = Keyboard::N;
    mapperCodeKeyWord[14] = Keyboard::O;
    mapperCodeKeyWord[15] = Keyboard::P;
    mapperCodeKeyWord[16] = Keyboard::Q;
    mapperCodeKeyWord[17] = Keyboard::R;
    mapperCodeKeyWord[18] = Keyboard::S;
    mapperCodeKeyWord[19] = Keyboard::T;
    mapperCodeKeyWord[20] = Keyboard::U;
    mapperCodeKeyWord[21] = Keyboard::V;
    mapperCodeKeyWord[22] = Keyboard::W;
    mapperCodeKeyWord[23] = Keyboard::X;
    mapperCodeKeyWord[24] = Keyboard::Y;
    mapperCodeKeyWord[25] = Keyboard::Z;

    // Numbers
    mapperCodeKeyWord[26] = Keyboard::Num0;
    mapperCodeKeyWord[27] = Keyboard::Num1;
    mapperCodeKeyWord[28] = Keyboard::Num2;
    mapperCodeKeyWord[29] = Keyboard::Num3;
    mapperCodeKeyWord[30] = Keyboard::Num4;
    mapperCodeKeyWord[31] = Keyboard::Num5;
    mapperCodeKeyWord[32] = Keyboard::Num6;
    mapperCodeKeyWord[33] = Keyboard::Num7;
    mapperCodeKeyWord[34] = Keyboard::Num8;
    mapperCodeKeyWord[35] = Keyboard::Num9;

    // Control keywords
    mapperCodeKeyWord[36] = Keyboard::Escape;
    mapperCodeKeyWord[37] = Keyboard::LControl;
    mapperCodeKeyWord[38] = Keyboard::LShift;
    mapperCodeKeyWord[39] = Keyboard::LAlt;
    mapperCodeKeyWord[40] = Keyboard::LSystem;
    mapperCodeKeyWord[41] = Keyboard::RControl;
    mapperCodeKeyWord[42] = Keyboard::RShift;
    mapperCodeKeyWord[43] = Keyboard::RAlt;
    mapperCodeKeyWord[44] = Keyboard::RSystem;
    mapperCodeKeyWord[45] = Keyboard::Menu;
    mapperCodeKeyWord[46] = Keyboard::LBracket;
    mapperCodeKeyWord[47] = Keyboard::LBracket;
    mapperCodeKeyWord[48] = Keyboard::Semicolon;
    mapperCodeKeyWord[49] = Keyboard::Comma;
    mapperCodeKeyWord[50] = Keyboard::Period;
    mapperCodeKeyWord[51] = Keyboard::Quote;
    mapperCodeKeyWord[52] = Keyboard::Slash;
    mapperCodeKeyWord[53] = Keyboard::BackSlash;
    mapperCodeKeyWord[54] = Keyboard::Tilde;
    mapperCodeKeyWord[55] = Keyboard::Equal;
    mapperCodeKeyWord[56] = Keyboard::Hyphen;
    mapperCodeKeyWord[57] = Keyboard::Space;
    mapperCodeKeyWord[58] = Keyboard::Enter;
    mapperCodeKeyWord[59] = Keyboard::Backspace;
    mapperCodeKeyWord[60] = Keyboard::Tab;
    mapperCodeKeyWord[61] = Keyboard::PageUp;
    mapperCodeKeyWord[62] = Keyboard::PageDown;
    mapperCodeKeyWord[63] = Keyboard::End;
    mapperCodeKeyWord[64] = Keyboard::Home;
    mapperCodeKeyWord[65] = Keyboard::Insert;
    mapperCodeKeyWord[66] = Keyboard::Delete;
    mapperCodeKeyWord[67] = Keyboard::Add;
    mapperCodeKeyWord[68] = Keyboard::Subtract;
    mapperCodeKeyWord[69] = Keyboard::Multiply;
    mapperCodeKeyWord[70] = Keyboard::Divide;
    mapperCodeKeyWord[71] = Keyboard::Left;
    mapperCodeKeyWord[72] = Keyboard::Right;
    mapperCodeKeyWord[73] = Keyboard::Up;
    mapperCodeKeyWord[74] = Keyboard::Down;

}


