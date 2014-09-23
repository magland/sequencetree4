
#ifndef slist_H
#define slist_H

//#include <QList>
//#define SList QList

#ifdef WIN32
#ifdef ST_SIMULATOR_LIB
	#define ST_EXPORT __declspec(dllexport)
#else
	#define ST_EXPORT 
#endif
#else
	#define ST_EXPORT
#endif


#define USING_SLIST


class SListNode {
public:
	void *data;
	SListNode *next;
	SListNode *prev;
};



template <typename TType>
class ST_EXPORT SList { //need to export at least one symbol so that .lib file will be created
public:
	SList() {initialize();}
	SList(const SList<TType> &X) {
		initialize();
		append(X);
	}
	virtual ~SList() {
		clear();
	}
	void clear() {
		SListNode *ptr=m_first;
		while (ptr) {
			SListNode *hold=ptr->next;
			delete ((TType *)ptr->data);
			delete ptr;
			ptr=hold;
		}
		initialize();
	}
	void initialize() {
		m_first=m_last=m_current=0;
		m_current_index=-1;
		m_count=0;
	}
	TType &operator[](long index) {
		goto_index(index);
		return (*((TType *)m_current->data));
	}
	void append(TType entry) {
		if (m_count==0) {
		m_first=m_last=new SListNode;
		m_first->data=new TType;
		(*((TType *)m_first->data))=entry;
		m_first->next=m_first->prev=0;
		m_current=m_first;
		m_current_index=0;
		m_count=1;
	}
	else {
		m_last->next=new SListNode;
		m_last->next->data=new TType;
		m_last->next->prev=m_last;
		m_last=m_last->next;
		(*((TType *)m_last->data))=entry;
		m_last->next=0;
		m_count++;
	}
	}
	void append(const SList<TType> &X) {
		SListNode *ptr=X.m_first;
		while (ptr) {
			append((*((TType *)ptr->data)));
			ptr=ptr->next;
		}
	}
	long count() const {
		return m_count;
	}
	bool isEmpty() {
		return m_count==0;
	}
	void removeAt(long index) {
		bool isfirst=(m_current==m_first);
		bool islast=(m_current==m_last);
		goto_index(index);
		SListNode *prev=m_current->prev;
		SListNode *next=m_current->next;
		delete ((TType *)m_current->data);
		delete m_current;
		if (prev) prev->next=next;
		if (next) next->prev=prev;
		m_count--;
		if (prev) {
			m_current=prev;
			m_current_index--;
		}
		else if (next) {
			m_current=next;
		}
		else initialize();
		if (isfirst) {
			m_first=next;
		}
		if (islast) {
			m_last=prev;
		}
	}
	void operator=(const SList<TType> &X) {
		clear();
		append(X);
	}
	TType value(long index) {
		if ((index<0)||(index>=count())) {
			TType dummy;
			return dummy;
		}
		return (*this)[index];
	}
	void operator<<(const TType entry) {append(entry);}
private:
	
	
	void goto_prev() {
		m_current=m_current->prev; m_current_index--;
	}
	void goto_next() {
		m_current=m_current->next; m_current_index++;
	}
	void goto_index(long index) {
		while (m_current_index>index)
			goto_prev();
		while (m_current_index<index)
			goto_next();
	}
	

	SListNode *m_first;
	SListNode *m_last;
	SListNode *m_current;
	long m_current_index;
	long m_count;
};





#endif
