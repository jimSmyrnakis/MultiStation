#include "Handle.hpp"

namespace MultiStation {

	namespace IO {

		Handle::Handle(void) : m_id(0) {

		}

		void Handle::Lock(void) {
			m_lock.lock();
		}

		bool Handle::TryLock(void){
			return m_lock.try_lock();
		}

		void Handle::Unlock(void){
			m_lock.unlock();
		}


		void Handle::SetNative(void* native){
			
			m_native = native;
		}

		void Handle::SetID(int id){
			m_id = id;
		}

		int Handle::GetID(void) const{
			return m_id;
		}

		void* Handle::GetNative(void) const{
			return m_native;
		}

	}

}
