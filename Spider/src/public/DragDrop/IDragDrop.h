#pragma once

class IDragDrop
{
    virtual bool IsDraggable() const = 0;
    virtual bool IsDropTarget() const = 0;
    virtual bool CanDrop(IDragDrop& item) = 0;
    virtual void DoDrop(IDragDrop& item) = 0;
};
