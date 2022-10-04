#include "Model/MoveRecord.h"


MoveRecord::MoveRecord(MoveRecord::Type type)
    : m_type(type)
{
}

MoveRecord::~MoveRecord()
{
}

MoveRecord::Type MoveRecord::getType() const
{
    return m_type;
}


DealMoveRecord::DealMoveRecord()
    :MoveRecord(MoveRecord::Type::Deal)
    , cardsTurned({ false })
    , packsRemoved({ false })
{
}

DealMoveRecord::DealMoveRecord(const DealMoveRecord& rec)
    : MoveRecord(MoveRecord::Type::Deal)
    , cardsTurned(rec.cardsTurned)
    , packsRemoved(rec.packsRemoved)
{
}


StackMoveRecord::StackMoveRecord()
    : MoveRecord(MoveRecord::Type::StackMove)
    , srcStack(-1)
    , srcCard(-1)
    , destStack(-1)
    , destCard(-1)
    , srcTurned(false)
    , destRemoved(false)
    , destTurned(false)
{
}

StackMoveRecord::StackMoveRecord(const StackMoveRecord& rec)
    : MoveRecord(MoveRecord::Type::StackMove)
    , srcStack(rec.srcStack)
    , srcCard(rec.srcCard)
    , destStack(rec.destStack)
    , destCard(rec.destCard)
    , srcTurned(rec.srcTurned)
    , destRemoved(rec.destRemoved)
    , destTurned(rec.destTurned)
{
}
