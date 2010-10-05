#pragma once

#define __PRIV( SIGTYPE ) private: SIGTYPE m_signal;
#define __CONNECT_FUNC( SIGTYPE ) void Connect( const SIGTYPE& slot ) { m_signal.connect( slot ); }
#define __REGISTER_SIGNAL( WRAPNAME, SIGTYPE, SIGNAME ) struct WRAPNAME { __CONNECT_FUNC( SIGTYPE ); __PRIV( SIGTYPE ) }; //WRAPNAME SIGNAME;

/*
		struct EnterSignal_Wrap {
			void Connect( const EnterSignal& slot ) {
				m_signal.connect( slot );
			}

			private:
				EnterSignal m_signal;
		} OnEnter;
*/
