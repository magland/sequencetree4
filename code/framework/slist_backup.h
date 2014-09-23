
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

template <typename Type>
class ST_EXPORT SList { //need to export at least one symbol so that .lib file will be created
public:
	SList();
	SList(const SList<Type> &X);
	virtual ~SList();
	void clear();
	void initialize();
	Type &operator[](long index);
	void append(Type entry);
	void append(const SList<Type> &X);
	long count() const;
	void removeAt(long index);
	void operator=(const SList<Type> &X);
	Type value(long index);
	void operator<<(const Type entry) {append(entry);}
private:
	struct Node {
		Type data;
		Node *next;
		Node *prev;
	};
	
	void goto_prev();
	void goto_next();
	void goto_index(long index);
	

	Node *m_first;
	Node *m_last;
	Node *m_current;
	long m_current_index;
	long m_count;
};

template <typename Type>
SList<Type>::SList() {
	initialize();
}

template <typename Type>
SList<Type>::SList(const SList<Type> &X) {
	initialize();
	append(X);
}

template <typename Type>
SList<Type>::~SList() {
	clear();
}

template <typename Type>
void SList<Type>::clear() {
	Node *ptr=m_first;
	while (ptr) {
		Node *hold=ptr->next;
		delete ptr;
		ptr=hold;
	}
	initialize();
}

template <typename Type>
void SList<Type>::initialize() {
	m_first=m_last=m_current=0;
	m_current_index=-1;
	m_count=0;
}

template <typename Type>
Type &SList<Type>::operator[](long index) {
	goto_index(index);
	return m_current->data;
}

template <typename Type>
void SList<Type>::append(Type entry) {
	if (m_count==0) {
		m_first=m_last=new Node;
		m_first->data=entry;
		m_first->next=m_first->prev=0;
		m_current=m_first;
		m_current_index=0;
		m_count=1;
	}
	else {
		m_last->next=new Node;
		m_last->next->prev=m_last;
		m_last=m_last->next;
		m_last->data=entry;
		m_last->next=0;
		m_count++;
	}
}

template <typename Type>
void SList<Type>::append(const SList<Type> &X) {
	Node *ptr=X.m_first;
	while (ptr) {
		append(ptr->data);
		ptr=ptr->next;
	}
}

template <typename Type>
long SList<Type>::count() const {return m_count;}

template <typename Type>
void SList<Type>::removeAt(long index) {
	bool isfirst=(m_current==m_first);
	bool islast=(m_current==m_last);
	goto_index(index);
	Node *prev=m_current->prev;
	Node *next=m_current->next;
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

template <typename Type>
void SList<Type>::goto_prev() {m_current=m_current->prev; m_current_index--;}

template <typename Type>
void SList<Type>::goto_next() {m_current=m_current->next; m_current_index++;}

template <typename Type>
void SList<Type>::goto_index(long index) {
	while (m_current_index>index)
		goto_prev();
	while (m_current_index<index)
		goto_next();
}

template <typename Type>
void SList<Type>::operator=(const SList<Type> &X) {
	clear();
	append(X);
}

template <typename Type>
Type SList<Type>::value(long index) {
	if ((index<0)||(index>=count())) {
		Type dummy;
		return dummy;
	}
	return (*this)[index];
}


#endif
