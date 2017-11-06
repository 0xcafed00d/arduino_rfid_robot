#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

template <typename objType>
struct StateMachine {
  public:
	enum Phase_t { None, Enter, Update, Leave };

	StateMachine(objType* objPtr)
	    : m_currentState(0), m_gotoState(0), m_objPtr(objPtr), m_inUpdate(false) {
	}

	typedef void (objType::*stateFunction_t)(Phase_t phase);

	void stateUpdate() {
		if (m_currentState) {
			m_inUpdate = true;
			(m_objPtr->*m_currentState)(Update);
			m_inUpdate = false;

			if (m_gotoState) {
				stateGoto(m_gotoState, m_gotoParam);
				m_gotoState = 0;
			}
		}
	}

	void stateGoto(stateFunction_t state, uint32_t param = 0) {
		if (m_inUpdate) {
			m_gotoState = state;
			m_gotoParam = param;
			return;
		}

		m_currentParam = param;
		if (m_currentState) {
			(m_objPtr->*m_currentState)(Leave);
		}

		m_currentState = state;

		if (m_currentState) {
			(m_objPtr->*m_currentState)(Enter);
		}
	}

	uint32_t stateParam() {
		return m_currentParam;
	}

  private:
	stateFunction_t m_currentState;
	uint32_t m_currentParam;

	stateFunction_t m_gotoState;
	uint32_t m_gotoParam;

	objType* m_objPtr;
	bool m_inUpdate;
};

#endif /* STATE_MACHINE_H */
