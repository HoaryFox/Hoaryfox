/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2015 Marco Costalba, Joona Kiiski, Tord Romstad
  Copyright (C) 2015-2019 Marco Costalba, Joona Kiiski, Gary Linscott, Tord Romstad

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include <map>
#include <string>

#include "types.h"

class Position;

namespace UCI {

class Option;

/// Custom comparator because UCI options should be case insensitive
struct CaseInsensitiveLess {
  bool operator() (const std::string&, const std::string&) const;
};

/// Our options container is actually a std::map
typedef std::map<std::string, Option, CaseInsensitiveLess> OptionsMap;

/// Option class implements an option as defined by UCI protocol
class Option {

  typedef void (*OnChange)(const Option&);

public:
  Option(OnChange = nullptr);
  Option(bool v, OnChange = nullptr);
  Option(const char* v, OnChange = nullptr);
  Option(double v, int minv, int maxv, OnChange = nullptr);
  Option(const char* v, const char* cur, OnChange = nullptr);

  Option& operator=(const std::string&);
  void operator<<(const Option&);
  operator double() const;
  operator std::string() const;
  bool operator==(const char*) const;

private:
  friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

  std::string defaultValue, currentValue, type;
  int min, max;
  size_t idx;
  OnChange on_change;
};

void init(OptionsMap&);
void loop(int argc, char* argv[]);
std::string value(Value v);
std::string square(Square s);
std::string move(Move m, bool chess960);
std::string pv(const Position& pos, Depth depth, Value alpha, Value beta);
Move to_move(const Position& pos, std::string& str);

// �]���֐���ǂݍ��񂾂��̃t���O�B�����evaldir�̕ύX�ɂƂ��Ȃ���false�ɂ���B	
extern bool load_eval_finished; // = false;

} // namespace UCI

extern UCI::OptionsMap Options;

// USI��"isready"�R�}���h���Ăяo���ꂽ�Ƃ��̏����B���̂Ƃ��ɕ]���֐��̓ǂݍ��݂Ȃǂ��s�Ȃ��B	
// benchmark�R�}���h�̃n���h���Ȃǂ�"isready"�����Ă��Ȃ��Ƃ��ɕ]���֐���ǂݍ��܂������Ƃ��ɗp����B	
// skipCorruptCheck == true�̂Ƃ��͕]���֐���2�x�ڂ̓ǂݍ��݂̂Ƃ���check sum�ɂ�郁�����j���`�F�b�N���ȗ�����B	
// ���@���̊֐��́AStockfish�ɂ͂Ȃ����Ȃ��ƕs�ւȂ̂Œǉ����Ă����B	
void is_ready(bool skipCorruptCheck = false);	
extern const char* StartFEN;

#endif // #ifndef UCI_H_INCLUDED
