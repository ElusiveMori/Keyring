#include "KeyRing.h"

using namespace ConState;

Base::Base(KeyRing& _master) : m_master(_master),
                               m_interface(_master.GetInterface()),
							   m_manager(_master.GetManager()) {
}

void Base::SetFinished(bool b) {
	m_finished = b;
}

bool Base::IsFinished() const {
	return m_finished;
}