/// ----------------------------------------------------------------------------------------------------------- end of chess.cpp


enum class eType { Queen, Rook, Knight, Bishop, Pawn } mType;
enum class eOwner { White, Black, None } mOwner;

class cPosition
{
	eOwner mOwner;
	eType mType;
public:
	eOwner GetOwner() const { return mOwner; }
	void SetOwner(eOwner Owner) { mOwner=Owner; }
	eType GetType() const { return mType; }
	void SetType(eType Type) { mType=Type; }
};

struct cBoard
{
	cPosition mPositions[16];

	cPosition &Position(int x, int y)
	{
		return mPositions[x+(y<<2)];
	}
	void Clear()
	{
		for(int i=0; i<16; ++i)
		{
			mPositions[i].SetOwner(eOwner::None);
		}
	}
};

cBoard board;

eType TypeMapper[256];

enum class eMoveResult { Win, Lose, Draw };

int max_level;

eMoveResult CheckLevel(int level);

eMoveResult CheckMove(eOwner Owner, int level, int sx, int sy, cPosition &TargetPosition, eType PromotionType)
{
	if(TargetPosition.GetOwner()!=eOwner::None&&TargetPosition.GetType()==eType::Queen)
		return eMoveResult::Win;
	if(level==max_level)
		return eMoveResult::Draw;
	++level;
	cPosition &SourcePosition=board.Position(sx, sy);
	auto MovedType=SourcePosition.GetType();
	SourcePosition.SetOwner(eOwner::None);

	auto OldTargetPosition=TargetPosition;
	TargetPosition.SetOwner(Owner);
	TargetPosition.SetType(PromotionType);

	auto Result=CheckLevel(level);

	TargetPosition=OldTargetPosition;
	SourcePosition.SetType(MovedType);
	SourcePosition.SetOwner(Owner);

	switch(Result)
	{
	case eMoveResult::Draw:
		break;
	case eMoveResult::Lose:
		return eMoveResult::Win;
	case eMoveResult::Win:
		return eMoveResult::Lose;
	}
	return eMoveResult::Draw;
}

eMoveResult CheckMove(eOwner Owner, int level, int sx, int sy, cPosition &TargetPosition)
{
	cPosition &SourcePosition=board.Position(sx, sy);
	auto MovedType=SourcePosition.GetType();
	return CheckMove(Owner, level, sx, sy, TargetPosition, MovedType);
}


#define CALL_CHECK_MOVE \
		switch(CheckMove(Owner, level, x, y, Position)) \
		{ \
		case eMoveResult::Win: \
			return eMoveResult::Win; \
		case eMoveResult::Draw: \
			AllLose=false; \
			break; \
		case eMoveResult::Lose: \
			break; \
		}


#define CALL_CHECK_MOVE_PROMOTE(PromotionType) \
		switch(CheckMove(Owner, level, x, y, Position, PromotionType)) \
		{ \
		case eMoveResult::Win: \
			return eMoveResult::Win; \
		case eMoveResult::Draw: \
			AllLose=false; \
			break; \
		case eMoveResult::Lose: \
			break; \
		}



eMoveResult CheckMove_Bishop(eOwner Owner, int level, int x, int y)
{
	bool AllLose=true;
	for(int ix=x-1, iy=y-1; ix>=0&&iy>=0; --ix, --iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x-1, iy=y+1; ix>=0&&iy<=3; --ix, ++iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1, iy=y-1; ix<=3&&iy>=0; ++ix, --iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1, iy=y+1; ix<=3&&iy<=3; ++ix, ++iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	return AllLose?eMoveResult::Lose:eMoveResult::Draw;
}

eMoveResult CheckMove_Knight(eOwner Owner, int level, int x, int y)
{
#define CHECK(dx, dy) \
	{ \
		int ix=x+dx, iy=y+dy; \
		if(ix>=0&&ix<=3&&iy>=0&&iy<=3) \
		{ \
			cPosition &Position=board.Position(ix, iy); \
			if(Position.GetOwner()!=Owner) \
			{ \
				CALL_CHECK_MOVE \
			} \
		} \
	} 
	bool AllLose=true;

	CHECK(-1, -2)
		CHECK(-2, -1)
		CHECK(1, -2)
		CHECK(2, -1)
		CHECK(-2, 1)
		CHECK(-1, 2)
		CHECK(2, 1)
		CHECK(1, 2)

		return AllLose?eMoveResult::Lose:eMoveResult::Draw;
}

eMoveResult CheckMove_Rook(eOwner Owner, int level, int x, int y)
{
	bool AllLose=true;
	for(int ix=x-1; ix>=0; --ix)
	{
		cPosition &Position=board.Position(ix, y);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1; ix<=3; ++ix)
	{
		cPosition &Position=board.Position(ix, y);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int iy=y-1; iy>=0; --iy)
	{
		cPosition &Position=board.Position(x, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int iy=y+1; iy<=3; ++iy)
	{
		cPosition &Position=board.Position(x, iy);
		if(Position.GetOwner()==Owner)
			break;
		CALL_CHECK_MOVE
			if(Position.GetOwner()!=eOwner::None)
				break;
	}
	return AllLose?eMoveResult::Lose:eMoveResult::Draw;
}

eMoveResult CheckMove_Queen(eOwner Owner, int level, int x, int y)
{
	switch(CheckMove_Rook(Owner, level, x, y))
	{
	case eMoveResult::Win:
		return eMoveResult::Win;
	case eMoveResult::Lose:
		return CheckMove_Bishop(Owner, level, x, y);
	case eMoveResult::Draw:
		{
			if(CheckMove_Bishop(Owner, level, x, y)==eMoveResult::Win)
				return eMoveResult::Win;
			return eMoveResult::Draw;
		}
	}
	return eMoveResult::Draw;
}

eMoveResult CheckMove_Pawn(eOwner Owner, int level, int x, int y)
{
	int ydir=Owner==eOwner::White?1:-1;
	int target_y=y+ydir;
	if(target_y<0||target_y>=4)
	{
		for(;;); // impossible, because would be queen
		return eMoveResult::Draw; 
	}
	bool AllLose=true;
	// forward move:
	{
		cPosition &Position=board.Position(x, target_y);
		if(Position.GetOwner()==eOwner::None)
		{
			if(target_y==0||target_y==3)
			{
				CALL_CHECK_MOVE_PROMOTE(eType::Rook)
				CALL_CHECK_MOVE_PROMOTE(eType::Knight)
				CALL_CHECK_MOVE_PROMOTE(eType::Bishop)
			}
			else
			{
				CALL_CHECK_MOVE
			}
		}
	}
	// hit:
	{
		int tx=x+1;
		if(tx>=0&&tx<=3)
		{
			cPosition &Position=board.Position(x, target_y);
			if(Position.GetOwner()!=eOwner::None&&Position.GetOwner()!=Owner)
			{
				if(target_y==0||target_y==3)
				{
					CALL_CHECK_MOVE_PROMOTE(eType::Rook)
						CALL_CHECK_MOVE_PROMOTE(eType::Knight)
						CALL_CHECK_MOVE_PROMOTE(eType::Bishop)
				}
				else
				{
					CALL_CHECK_MOVE
				}
			}
		}
	}
	{
		int tx=x+-1;
		if(tx>=0&&tx<4)
		{
			cPosition &Position=board.Position(x, target_y);
			if(Position.GetOwner()!=eOwner::None&&Position.GetOwner()!=Owner)
			{
				if(target_y==0||target_y==3)
				{
					CALL_CHECK_MOVE_PROMOTE(eType::Rook)
					CALL_CHECK_MOVE_PROMOTE(eType::Knight)
					CALL_CHECK_MOVE_PROMOTE(eType::Bishop)
				}
				else
				{
					CALL_CHECK_MOVE
				}
			}
		}
	}
	return AllLose?eMoveResult::Lose:eMoveResult::Draw;
}


eMoveResult CheckLevel(int level)
{
	eOwner CheckedOwner=(level&1)?eOwner::Black:eOwner::White;
	bool AllLose=true;
	for(int x=0; x<4; ++x)
	{
		for(int y=0; y<4; ++y)
		{
			cPosition &Position=board.Position(x, y);
			if(Position.GetOwner()==CheckedOwner)
			{
				eMoveResult Result;
				switch(Position.GetType())
				{
				case eType::Bishop:
					Result=CheckMove_Bishop(CheckedOwner, level, x, y);
					break;
				case eType::Knight:
					Result=CheckMove_Knight(CheckedOwner, level, x, y);
					break;
				case eType::Queen:
					Result=CheckMove_Queen(CheckedOwner, level, x, y);
					break;
				case eType::Rook:
					Result=CheckMove_Rook(CheckedOwner, level, x, y);
					break;
				case eType::Pawn:
					Result=CheckMove_Pawn(CheckedOwner, level, x, y);
					break;
				}
				if(Result==eMoveResult::Win)
					return eMoveResult::Win;
				if(Result==eMoveResult::Draw)
					AllLose=false;
			}
		}
	}
	return AllLose?eMoveResult::Lose:eMoveResult::Draw;
}

void Solve()
{
	int w, b;
	scanf("%d %d %d", &w, &b, &max_level);
	--max_level;
	max_level&=~1;

	auto ReadPiece=[](eOwner Owner)
	{
		char t[2], x[2];
		int y;
		scanf("%s %s %d", t, x, &y);
		auto &Position=board.Position(*x-'A', y-1);
		Position.SetType(TypeMapper[*t]);
		Position.SetOwner(Owner);
	};
	board.Clear();

	for(int i=0; i<w; ++i)
	{
		ReadPiece(eOwner::White);
	}
	for(int i=0; i<b; ++i)
	{
		ReadPiece(eOwner::Black);
	}
	if(CheckLevel(0)==eMoveResult::Win)
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
	}
}

void Init()
{
	TypeMapper['R']=eType::Rook;
	TypeMapper['Q']=eType::Queen;
	TypeMapper['N']=eType::Knight;
	TypeMapper['B']=eType::Bishop;
	TypeMapper['P']=eType::Pawn;
}

/// ----------------------------------------------------------------------------------------------------------- end of chess.cpp

///    ALTERNATE SOLUTION, checking for "check"






#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <string>
#include <set>
#include <tuple>
#include <functional>
#include <cstring>
#include <deque>
#include <array>
#include <map>
#include <random>
#include <unordered_map>

#undef max
#undef min

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#else
char *crash_please=(char *)42;
#define ASSERT(x) 
//if(!(x)) { printf("%s failed",#x); *crash_please=33; }
#endif

#include <chrono>

class cLogPerformance_Guard
{
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime=std::chrono::high_resolution_clock::now();
	const char *mName;
public:
	cLogPerformance_Guard(const char *Name): mName(Name) {}
	~cLogPerformance_Guard()
	{
		auto EndTime=std::chrono::high_resolution_clock::now();
		auto Elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(EndTime-mStartTime);
		//		printf("--- Elapsed %llu ms in %s ---\n", (unsigned long long)Elapsed.count(), mName);
	}
};

using namespace std;
using namespace std::string_literals;
using ull=unsigned long long;
using ll=long long;
constexpr ll mod=1'000'000'007;
template<class I> auto rev(I i) { return std::reverse_iterator<I>(i); }

#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RIIII(var_name1, var_name2, var_name3, var_name4) int var_name1, var_name2, var_name3, var_name4; scanf("%d %d %d %d", &var_name1, &var_name2, &var_name3, &var_name4);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define RLLLL(var_name1, var_name2, var_name3, var_name4) ll var_name1, var_name2, var_name3, var_name4; scanf("%lld %lld %lld %lld", &var_name1, &var_name2, &var_name3, &var_name4);
#define RD(var_name) double var_name; scanf("%lf", &var_name);
#define RDV(var_name, size) vector<double> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RDD(var_name1, var_name2) double var_name1, var_name2; scanf("%lf %lf", &var_name1, &var_name2);
#define RDDD(var_name1, var_name2, var_name3) double var_name1, var_name2, var_name3; scanf("%lf %lf %lf", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(int var=0;var<max_value;++var)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class eType { Queen, Rook, Knight, Bishop, Pawn, King } mType;
enum class eOwner { White, Black, None } mOwner;

class cPosition
{
	eOwner mOwner;
	eType mType;
public:
	eOwner GetOwner() const { return mOwner; }
	void SetOwner(eOwner Owner) { mOwner=Owner; }
	eType GetType() const { return mType; }
	void SetType(eType Type) { mType=Type; }
};

struct cPiece
{
	eType type;
	int x, y;
	cPiece()=default;
	cPiece(eType type, int x, int y): type(type), x(x), y(y) {}
};

vector<cPiece> black_pieces, white_pieces;
int black_king_x, black_king_y, white_king_x, white_king_y;
int ways_check;

struct cBoard
{
	cPosition mPositions[64];

	cPosition &Position(int x, int y)
	{
		return mPositions[x+(y<<3)];
	}
	void Clear()
	{
		for(int i=0; i<64; ++i)
		{
			mPositions[i].SetOwner(eOwner::None);
		}
	}
};

cBoard board;

eType TypeMapper[256];



bool CheckCheck(eOwner KingOwner)
{
	int x=KingOwner==eOwner::Black?black_king_x:white_king_x, y=KingOwner==eOwner::Black?black_king_y:white_king_y;

#define POSITION_CHECK(CheckedType1, CheckedType2) \
		cPosition& p=board.Position(ix, iy); \
		if(p.GetOwner()==KingOwner)	\
			break; \
		if(p.GetOwner()!=eOwner::None) \
		{ \
			if(p.GetType()==CheckedType1||p.GetType()==CheckedType2) \
				return true; \
			break; \
		}

	// check rook:
	for(int ix=x-1, iy=y; ix>=0; --ix)
	{
		POSITION_CHECK(eType::Rook, eType::Queen);
	}
	for(int ix=x+1, iy=y; ix<=7; ++ix)
	{
		POSITION_CHECK(eType::Rook, eType::Queen);
	}
	for(int ix=x, iy=y-1; iy>=0; --iy)
	{
		POSITION_CHECK(eType::Rook, eType::Queen);
	}
	for(int ix=x, iy=y+1; iy<=7; ++iy)
	{
		POSITION_CHECK(eType::Rook, eType::Queen);
	}
	// check bishop
	for(int ix=x-1, iy=y-1; ix>=0&&iy>=0; --ix, --iy)
	{
		POSITION_CHECK(eType::Bishop, eType::Queen);
	}
	for(int ix=x-1, iy=y+1; ix>=0&&iy<=7; --ix, ++iy)
	{
		POSITION_CHECK(eType::Bishop, eType::Queen);
	}
	for(int ix=x+1, iy=y-1; ix<=7&&iy>=0; ++ix, --iy)
	{
		POSITION_CHECK(eType::Bishop, eType::Queen);
	}
	for(int ix=x+1, iy=y+1; ix<=7&&iy<=7; ++ix, ++iy)
	{
		POSITION_CHECK(eType::Bishop, eType::Queen);
	}
	// check knight
#define CHECK(dx, dy) \
	{ \
		int ix=x+dx, iy=y+dy; \
		if(ix>=0&&ix<=7&&iy>=0&&iy<=7) \
		{ \
			cPosition& p=board.Position(ix, iy); \
			if(p.GetOwner()==eOwner::White) \
			{ \
				if(p.GetType()==eType::Knight) \
					return true; \
			} \
		} \
	} 

	CHECK(-1, -2)
		CHECK(-2, -1)
		CHECK(1, -2)
		CHECK(2, -1)
		CHECK(-2, 1)
		CHECK(-1, 2)
		CHECK(2, 1)
		CHECK(1, 2)


#undef CHECK



	return false;
}

bool CheckMove(eOwner Owner, int sx, int sy, cPosition &TargetPosition, eType PromotionType)
{
	cPosition &SourcePosition=board.Position(sx, sy);
	auto MovedType=SourcePosition.GetType();
	SourcePosition.SetOwner(eOwner::None);

	auto OldTargetPosition=TargetPosition;
	TargetPosition.SetOwner(Owner);
	TargetPosition.SetType(PromotionType);

	bool is_check=(!CheckCheck(eOwner::White))&&CheckCheck(eOwner::Black);

	TargetPosition=OldTargetPosition;
	SourcePosition.SetType(MovedType);
	SourcePosition.SetOwner(Owner);

	return is_check;
}

bool CheckMove(eOwner Owner, int sx, int sy, cPosition &TargetPosition)
{
	cPosition &SourcePosition=board.Position(sx, sy);
	auto MovedType=SourcePosition.GetType();
	return CheckMove(Owner, sx, sy, TargetPosition, MovedType);
}


#define CALL_CHECK_MOVE \
		CheckMove(Owner, x, y, Position)


#define CALL_CHECK_MOVE_PROMOTE(PromotionType) \
		CheckMove(Owner, x, y, Position, PromotionType)



void CheckMove_Bishop(eOwner Owner, int x, int y)
{
	bool AllLose=true;
	for(int ix=x-1, iy=y-1; ix>=0&&iy>=0; --ix, --iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
			break;
	}
	for(int ix=x-1, iy=y+1; ix>=0&&iy<=7; --ix, ++iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1, iy=y-1; ix<=7&&iy>=0; ++ix, --iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1, iy=y+1; ix<=7&&iy<=7; ++ix, ++iy)
	{
		cPosition &Position=board.Position(ix, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
}

void CheckMove_Knight(eOwner Owner, int x, int y)
{
#define CHECK(dx, dy) \
	{ \
		int ix=x+dx, iy=y+dy; \
		if(ix>=0&&ix<=7&&iy>=0&&iy<=7) \
		{ \
			cPosition &Position=board.Position(ix, iy); \
			if(Position.GetOwner()!=Owner) \
			{ \
				if(CALL_CHECK_MOVE) ++ways_check; \
			} \
		} \
	} 

	CHECK(-1, -2)
	CHECK(-2, -1)
	CHECK(1, -2)
	CHECK(2, -1)
	CHECK(-2, 1)
	CHECK(-1, 2)
	CHECK(2, 1)
	CHECK(1, 2)

}

void CheckMove_Rook(eOwner Owner, int x, int y)
{
	bool AllLose=true;
	for(int ix=x-1; ix>=0; --ix)
	{
		cPosition &Position=board.Position(ix, y);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int ix=x+1; ix<=7; ++ix)
	{
		cPosition &Position=board.Position(ix, y);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int iy=y-1; iy>=0; --iy)
	{
		cPosition &Position=board.Position(x, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
	for(int iy=y+1; iy<=7; ++iy)
	{
		cPosition &Position=board.Position(x, iy);
		if(Position.GetOwner()==Owner)
			break;
		if(CALL_CHECK_MOVE)
		{
			++ways_check;
		}
		if(Position.GetOwner()!=eOwner::None)
				break;
	}
}

void CheckMove_Queen(eOwner Owner, int x, int y)
{
	CheckMove_Rook(Owner, x, y);
	CheckMove_Bishop(Owner, x, y);
}

void CheckMove_Pawn(eOwner Owner, int x, int y)
{
	int ydir=Owner==eOwner::White?-1:1;
	int target_y=y+ydir;
	// forward move:
	{
		cPosition &Position=board.Position(x, target_y);
		if(Position.GetOwner()==eOwner::None)
		{
			if(target_y==0||target_y==7)
			{
				int q=0;
				if(CALL_CHECK_MOVE_PROMOTE(eType::Rook))
				{
					q=1;
					++ways_check;
				}
				if(CALL_CHECK_MOVE_PROMOTE(eType::Knight))
					++ways_check;
				if(CALL_CHECK_MOVE_PROMOTE(eType::Bishop))
				{
					q=1;
					++ways_check;
				}
				ways_check+=q;
			}
			else
			{
				if(CALL_CHECK_MOVE)
					++ways_check;
			}
		}
	}
}

char buf[10];

void Solve()
{
	ways_check=0;
	black_pieces.clear();
	white_pieces.clear();
	board.Clear();
	FOR(r, 8)
	{
		scanf("%s", buf);
		FOR(c, 8)
		{
			if(buf[c]=='#')
				continue;
			auto& p=board.Position(c, r);
			p.SetType(TypeMapper[buf[c]]);
			if(buf[c]>='a'&&buf[c]<='z')
			{
				p.SetOwner(eOwner::Black);
				black_pieces.emplace_back(p.GetType(), c, r);
				if(p.GetType()==eType::King)
				{
					black_king_x=c; black_king_y=r;
				}
			}
			else
			{
				p.SetOwner(eOwner::White);
				white_pieces.emplace_back(p.GetType(), c, r);
				if(p.GetType()==eType::King)
				{
					white_king_x=c; white_king_y=r;
				}
			}
		}
	}
	for(auto& piece: white_pieces)
	{
		int x=piece.x, y=piece.y;
		switch(piece.type)
		{
// 		case eType::Bishop:
// 			CheckMove_Bishop(eOwner::White, x, y);
// 			break;
// 		case eType::Knight:
// 			CheckMove_Knight(eOwner::White, x, y);
// 			break;
// 		case eType::Queen:
// 			CheckMove_Queen(eOwner::White, x, y);
// 			break;
// 		case eType::Rook:
// 			CheckMove_Rook(eOwner::White, x, y);
// 			break;
		case eType::Pawn:
			CheckMove_Pawn(eOwner::White, x, y);
			break;
		}
	}
	printf("%d\n", ways_check);
}

int Init()
{
	TypeMapper['r']=TypeMapper['R']=eType::Rook;
	TypeMapper['q']=TypeMapper['Q']=eType::Queen;
	TypeMapper['n']=TypeMapper['N']=eType::Knight;
	TypeMapper['b']=TypeMapper['B']=eType::Bishop;
	TypeMapper['p']=TypeMapper['P']=eType::Pawn;
	TypeMapper['k']=TypeMapper['K']=eType::King;

//	int t=1;
	RI(t);
	return t;
}
int main()
{
	//	std::ios::sync_with_stdio(false);
	int t=Init();
	for(int tc=1; tc<=t; ++tc)
	{
		//		printf("Case #%d: ", tc);
		Solve();
	}
}
