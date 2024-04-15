/*
 * UML_FSM.h
 *
 *  Created on: Apr 10, 2024
 *      Author: pavel
 */

#ifndef INC_UML_FSM_H_
#define INC_UML_FSM_H_

#include "stdint.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "queue.h"
#include <array>
#include <queue>
#include <unordered_map>
//#define NUM_STATES 10
//#define NUM_EVENTS 5
//#define NUM_MACHINES 2

// Forward declaration of classes
class BaseState;
class StandardStates;
class EventStates;
class ChoiseStates;

// Define stateType and constants
using stateType = signed char;
const stateType NUM_STATES = 10;
const stateType NUM_EVENTS = 10;
const stateType NUM_MACHINES = 5;

// Define StateFunction type
using StateFunction = void (*)();

// TransitionTable class definition
class TransitionTable {
public:
    TransitionTable();

    std::array<StateFunction, NUM_STATES> stateFunctions;
    std::array<osMessageQueueId_t, NUM_STATES> stateQueueMappings;
    std::queue<stateType> eventQueue;

private:
    stateType num_states;
    stateType num_events;
};

// FiniteStateMachine class definition
class FiniteStateMachine {
public:
    FiniteStateMachine();
    ~FiniteStateMachine();

    StandardStates createStateStandart(stateType initial_state, stateType new_state, StateFunction functPtr);
    EventStates createStateEvent(stateType initial_state, StateFunction functPtr);
    ChoiseStates createStateChoise(stateType initial_state, StateFunction functPtr);
    void setStartState(stateType state);
    void sendEvent(stateType event);
    void stateMachine();
    void next();
  /*  void next();*/

    friend class StandardStates;
    friend class EventStates;
    friend class ChoiseStates;

    BaseState* pState;
    // Определение типа словаря
    using StateMap = std::unordered_map<stateType, BaseState*>;
    // Создание пустого словаря
    StateMap stateDictionary;
private:
    stateType thisState;
    stateType next_state;
    stateType startState;
    TransitionTable transitionTable;
};

// BaseState class definition
class BaseState {
protected:
    stateType initial_state;
    stateType new_state;
    FiniteStateMachine* pFiniteStateMachine;
    StateFunction functPtr;

public:
    BaseState(stateType initial, stateType new_state, FiniteStateMachine* pFSM, StateFunction funct);
    virtual void next();
};

// ChoiseStates class definition
class ChoiseStates : public BaseState {
public:
    ChoiseStates(stateType initial_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr);

    void addChoise(stateType choise, stateType new_state);
    void setChoise(stateType choise);
    void goChoise();
    void next() override;

private:
    stateType thisChoise;
    std::array<stateType, NUM_STATES> stateChoise;
};

// StandardStates class definition
class StandardStates : public BaseState {
public:
    StandardStates(stateType initial_state, stateType new_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr);

    void end();
    void next () override;
};

// EventStates class definition
class EventStates : public BaseState {
public:
    EventStates(stateType initial_state, FiniteStateMachine* pFiniteStateMachine, StateFunction functPtr);

    void addEvent(stateType event, stateType new_state);
    void waitEvent();
    void next () override;

private:
    stateType handleTransition(stateType event);
    std::array<stateType, NUM_EVENTS> transitionEvent;
    std::queue<stateType>* eventQueuePtr;
};

#endif /* INC_UML_FSM_H_ */
