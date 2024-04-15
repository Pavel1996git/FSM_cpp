/*
 * UML_FSM.c
 *
 *  Created on: Apr 10, 2024
 *      Author: pavel
 */


#include "UML_FSM.hpp"
#include <array>
#include <queue>
using StateFunction = void(*)();
//volatile Queue eventQueue[NUM_MACHINES];

#include "UML_FSM.hpp"

// TransitionTable constructor
TransitionTable::TransitionTable() : num_states(NUM_STATES), num_events(NUM_EVENTS) {
    stateFunctions.fill(nullptr);
}

// FiniteStateMachine constructor
FiniteStateMachine::FiniteStateMachine() : thisState(-1), startState(-1) {
    transitionTable = TransitionTable();
}

// FiniteStateMachine destructor
FiniteStateMachine::~FiniteStateMachine() {}

// BaseState constructor
BaseState::BaseState(stateType initial, stateType new_state, FiniteStateMachine* pFSM, StateFunction funct)
    : initial_state(initial), new_state(new_state), pFiniteStateMachine(pFSM), functPtr(funct) {
	pFiniteStateMachine->stateDictionary.emplace(initial, this);
}
void BaseState::next()
{

}

// ChoiseStates constructor
ChoiseStates::ChoiseStates(stateType initial_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr)
    : BaseState(initial_state, -1, pFiniteStateMachine, functPtr) {
	pFiniteStateMachine->transitionTable.stateFunctions.at(initial_state) = functPtr;
    stateChoise.fill(-1);
}

// ChoiseStates method: addChoise
void ChoiseStates::addChoise(stateType choise, stateType new_state) {
    stateChoise.at(choise) = new_state;
}

// ChoiseStates method: setChoise
void ChoiseStates::setChoise(stateType choise) {
    thisChoise = choise;
}

// ChoiseStates method: goChoise
void ChoiseStates::goChoise() {
    if (pFiniteStateMachine->thisState == stateChoise.at(thisChoise))
        return;

    pFiniteStateMachine->thisState = stateChoise.at(thisChoise);
}

void ChoiseStates::next()
{
	goChoise();
}
// StandardStates constructor
StandardStates::StandardStates(stateType initial_state, stateType new_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr)
    : BaseState(initial_state, new_state, pFiniteStateMachine, functPtr) {
    pFiniteStateMachine->transitionTable.stateFunctions.at(initial_state) = functPtr;
}

// StandardStates method: end
void StandardStates::end() {

    pFiniteStateMachine->thisState = new_state;
}

void StandardStates::next ()
{
	end();
}
/*
// StandardStates method: end
void FiniteStateMachine::next() {
    this->thisState = new_state;
}
*/
// EventStates constructor
EventStates::EventStates(stateType initial_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr)
    : BaseState(initial_state, -1, pFiniteStateMachine, functPtr) {
    transitionEvent.fill(-1);
    eventQueuePtr = &(pFiniteStateMachine->transitionTable.eventQueue);
    pFiniteStateMachine->transitionTable.stateFunctions.at(initial_state) = functPtr;
}

// EventStates method: addEvent
void EventStates::addEvent(stateType event, stateType new_state) {
    transitionEvent.at(event) = new_state;
}

// EventStates method: waitEvent
void EventStates::waitEvent() {
	stateType currentEvent;
    while (1) {

        while (eventQueuePtr->empty()) {
            osDelay(1);
        }

       currentEvent = eventQueuePtr->front();
        if (handleTransition(currentEvent) != -1) {
            eventQueuePtr->pop();
            break;
        } else {
            osDelay(1);
        }
    }

    pFiniteStateMachine->thisState = handleTransition(currentEvent);
}
void EventStates::next()
{
	waitEvent();
}
// EventStates method: handleTransition
stateType EventStates::handleTransition(stateType event) {
    if (transitionEvent.at(event) != -1) {
        stateType newState = transitionEvent.at(event);
        return newState;
    } else {
        return -1;
    }
}


// FiniteStateMachine method: createStateStandart
StandardStates FiniteStateMachine::createStateStandart(stateType initial_state, stateType new_state, StateFunction functPtr) {
    return StandardStates(initial_state, new_state, this, functPtr);
}



// FiniteStateMachine method: createStateEvent
EventStates FiniteStateMachine::createStateEvent(stateType initial_state, StateFunction functPtr) {
    return EventStates(initial_state, this, functPtr);
}

// FiniteStateMachine method: createStateChoise
ChoiseStates FiniteStateMachine::createStateChoise(stateType initial_state, StateFunction functPtr) {
    return ChoiseStates(initial_state, this, functPtr);
}

// FiniteStateMachine method: sendEvent
void FiniteStateMachine::sendEvent(stateType event) {
    this->transitionTable.eventQueue.push(event);
}

// FiniteStateMachine method: setStartState
void FiniteStateMachine::setStartState(stateType state) {
    startState = state;
    //this->pState =
}

// FiniteStateMachine method: stateMashine
void FiniteStateMachine::stateMachine()
{
    this->thisState = startState;
    for(;;) {
    	this->pState = stateDictionary[thisState];
    	this->transitionTable.stateFunctions[this->thisState]();
    }
}
void FiniteStateMachine::next()
{
	pState->next();
}


/*


// An array mapping each state to its corresponding message queue ID, allowing communication between tasks based on their current states.
osMessageQueueId_t stateQueueMappings[NUM_STATES];


void createStateQueueMapping(stateType state, osMessageQueueId_t queueHandle)
{
    // Create the association between the state and the message queue
    stateQueueMappings[state] = queueHandle;
}
osMessageQueueId_t getQueueForState(int8_t state)
{
    // Retrieve the message queue associated with the state
    return stateQueueMappings[state];
}


void MessageQueueState(stateType state)
{
    // Send the state value to the message queue associated with the specified state
    xQueueSend(getQueueForState(state), &state, pdMS_TO_TICKS(DELAY_QUEUE));
}

void waitForOwnState(stateType currentState)
{
    // Wait to receive the current state from the associated message queue
    xQueueReceive(getQueueForState(currentState), &currentState, portMAX_DELAY);
}
*/
