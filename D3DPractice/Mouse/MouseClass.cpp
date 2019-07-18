#include "MouseClass.h"

void MouseClass::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	MouseEvent me(MouseEvent::EventType::RPress, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnMiddlePressed(int x, int y)
{
	this->mButtonDown = true;
	MouseEvent me(MouseEvent::EventType::MPress, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	MouseEvent me(MouseEvent::EventType::LRelease, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnRightReleased(int x, int y)
{
	this->rightIsDown = false;
	MouseEvent me(MouseEvent::EventType::RRelease, x, y);
	this->eventBuffer.push(me);
}
void MouseClass::OnMiddleReleased(int x, int y)
{
	this->mButtonDown = false;
	MouseEvent me(MouseEvent::EventType::MRelease, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnWheelUp(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseClass::OnWheelDown(int x, int y) {
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseClass::OnMouseMove(int x, int y) {
	this->x;
	this->y;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseClass::IsLeftDown() {
	return this->leftIsDown;
}

bool MouseClass::IsRightDown() {
	return this->rightIsDown;
}

bool MouseClass::IsMiddleDown() {
	return this->mButtonDown;
}

int MouseClass::GetPosX()
{
	return this->x;
}

int MouseClass::GetPosY()
{
	return this->y;
}

MousePoint MouseClass::GetPos()
{
	return { this->x,this->y };
}

bool MouseClass::EventBufferIsEmpty()
{
	return this->eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
		return MouseEvent();
	else {
		MouseEvent me = eventBuffer.front();
		eventBuffer.pop();
		return me;
	}
}
