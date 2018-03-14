// sigslot_simple.h
// a simplified sigsloat version with one argument

#ifndef SIGSLOT_SIMPLE_H__
#define SIGSLOT_SIMPLE_H__

#include <set>
#include <list>

#define SIGSLOT_DEFAULT_MT_POLICY single_threaded

namespace sigslot {

    class single_threaded
    {
    public:
        single_threaded()
        {
            ;
        }

        virtual ~single_threaded()
        {
            ;
        }

        void lock()
        {
            ;
        }

        void unlock()
        {
            ;
        }
    };

    template<class mt_policy>
    class lock_block
    {
    public:
        mt_policy *m_mutex;

        lock_block(mt_policy *mtx)
                : m_mutex(mtx)
        {
            m_mutex->lock();
        }

        ~lock_block()
        {
            m_mutex->unlock();
        }
    };

    template<class mt_policy>
    class has_slots;

    template<class arg1_type, class mt_policy>
    class _connection_base1
    {
    public:
        virtual ~_connection_base1() { }
        virtual has_slots<mt_policy>* getdest() const = 0;
        virtual void emit(arg1_type) = 0;
        virtual _connection_base1<arg1_type, mt_policy>* clone() = 0;
        virtual _connection_base1<arg1_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
    };

    template<class mt_policy>
    class _signal_base : public mt_policy
    {
    public:
        virtual void slot_disconnect(has_slots<mt_policy>* pslot) = 0;
        virtual void slot_duplicate(const has_slots<mt_policy>* poldslot, has_slots<mt_policy>* pnewslot) = 0;
    };

    template<class  mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
    class has_slots : public mt_policy
    {
    private:
        typedef typename std::set<_signal_base<mt_policy> *> sender_set;
        typedef typename sender_set::const_iterator const_iterator;

    public:
        has_slots()
        {
            ;
        }

        has_slots(const has_slots& hs)
                : mt_policy(hs)
        {
            lock_block<mt_policy> lock(this);
            const_iterator it = hs.m_senders.begin();
            const_iterator itEnd = hs.m_senders.end();

            while(it != itEnd)
            {
                (*it)->slot_duplicate(&hs, this);
                m_senders.insert(*it);
                ++it;
            }
        }

        void signal_connect(_signal_base<mt_policy>* sender)
        {
            lock_block<mt_policy> lock(this);
            m_senders.insert(sender);
        }

        void signal_disconnect(_signal_base<mt_policy>* sender)
        {
            lock_block<mt_policy> lock(this);
            m_senders.erase(sender);
        }

        virtual ~has_slots()
        {
            disconnect_all();
        }

        void disconnect_all()
        {
            lock_block<mt_policy> lock(this);
            const_iterator it = m_senders.begin();
            const_iterator itEnd = m_senders.end();

            while(it != itEnd)
            {
                (*it)->slot_disconnect(this);
                ++it;
            }

            m_senders.erase(m_senders.begin(), m_senders.end());
        }

    private:
        sender_set m_senders;
    };

    template<class arg1_type, class mt_policy>
    class _signal_base1 : public _signal_base<mt_policy>
    {
    public:
        typedef typename std::list<_connection_base1<arg1_type, mt_policy> *>  connections_list;
        typedef typename connections_list::const_iterator const_iterator;
        typedef typename connections_list::iterator iterator;

        _signal_base1()
        {
            ;
        }

        _signal_base1(const _signal_base1<arg1_type, mt_policy>& s)
                : _signal_base<mt_policy>(s)
        {
            lock_block<mt_policy> lock(this);
            const_iterator it = s.m_connected_slots.begin();
            const_iterator itEnd = s.m_connected_slots.end();

            while(it != itEnd)
            {
                (*it)->getdest()->signal_connect(this);
                m_connected_slots.push_back((*it)->clone());

                ++it;
            }
        }

        void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
        {
            lock_block<mt_policy> lock(this);
            iterator it = m_connected_slots.begin();
            iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                if((*it)->getdest() == oldtarget)
                {
                    m_connected_slots.push_back((*it)->duplicate(newtarget));
                }

                ++it;
            }
        }

        ~_signal_base1()
        {
            disconnect_all();
        }

        void disconnect_all()
        {
            lock_block<mt_policy> lock(this);
            const_iterator it = m_connected_slots.begin();
            const_iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                (*it)->getdest()->signal_disconnect(this);
                delete *it;

                ++it;
            }

            m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
        }

        void disconnect(has_slots<mt_policy>* pclass)
        {
            lock_block<mt_policy> lock(this);
            iterator it = m_connected_slots.begin();
            iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                if((*it)->getdest() == pclass)
                {
                    delete *it;
                    m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    return;
                }

                ++it;
            }
        }

        void slot_disconnect(has_slots<mt_policy>* pslot)
        {
            lock_block<mt_policy> lock(this);
            iterator it = m_connected_slots.begin();
            iterator itEnd = m_connected_slots.end();

            while(it != itEnd)
            {
                iterator itNext = it;
                ++itNext;

                if((*it)->getdest() == pslot)
                {
                    delete *it;
                    m_connected_slots.erase(it);
                }

                it = itNext;
            }
        }


    protected:
        connections_list m_connected_slots;
    };

    template<class dest_type, class arg1_type, class mt_policy>
    class _connection1 : public _connection_base1<arg1_type, mt_policy>
    {
    public:
        _connection1()
        {
            m_pobject = NULL;
            m_pmemfun = NULL;
        }

        _connection1(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type))
        {
            m_pobject = pobject;
            m_pmemfun = pmemfun;
        }

        virtual _connection_base1<arg1_type, mt_policy>* clone()
        {
            return new _connection1<dest_type, arg1_type, mt_policy>(*this);
        }

        virtual _connection_base1<arg1_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
        {
            return new _connection1<dest_type, arg1_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
        }

        virtual void emit(arg1_type a1)
        {
            (m_pobject->*m_pmemfun)(a1);
        }

        virtual has_slots<mt_policy>* getdest() const
        {
            return m_pobject;
        }

    private:
        dest_type* m_pobject;
        void (dest_type::* m_pmemfun)(arg1_type);
    };

    template<class arg1_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
    class signal1 : public _signal_base1<arg1_type, mt_policy>
    {
    public:
        typedef typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator const_iterator;
        signal1()
        {
            ;
        }

        signal1(const signal1<arg1_type, mt_policy>& s)
                : _signal_base1<arg1_type, mt_policy>(s)
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
        {
            lock_block<mt_policy> lock(this);
            _connection1<desttype, arg1_type, mt_policy>* conn =
                    new _connection1<desttype, arg1_type, mt_policy>(pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        void emit(arg1_type a1)
        {
            lock_block<mt_policy> lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while(it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1);

                it = itNext;
            }
        }

        void operator()(arg1_type a1)
        {
            emit(a1);
        }
    };
}; // namespace sigslot

#endif // SIGSLOT_SIMPLE_H__