#pragma once


#include <vector>
#include <array>


class MoveRecord
{
public:
    enum class Type { Deal, StackMove };

public:
    MoveRecord(Type type);
    ~MoveRecord();
    Type getType() const;

private:
    Type m_type;
};


class DealMoveRecord : public MoveRecord
{
public:
    DealMoveRecord();
    DealMoveRecord(const DealMoveRecord& rec);

    std::array<bool, 10> packsRemoved;
    std::array<bool, 10> cardsTurned;
};


class StackMoveRecord : public MoveRecord
{
public:
    StackMoveRecord();
    StackMoveRecord(const StackMoveRecord& rec);

public:
    int srcStack;
    int srcCard;
    int destStack;
    int destCard;
    bool srcTurned;
    bool destRemoved;
    bool destTurned;
};