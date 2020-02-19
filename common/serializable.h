#pragma once
#include <QDataStream>

class serializable{
public:
	friend QDataStream& operator << (QDataStream& str, const serializable &rhs);
	friend QDataStream& operator >> (QDataStream& str, serializable &rhs);
	virtual ~serializable(){};
	virtual qint8 get_magic_byte()const =0;
protected:
	virtual void serialize(QDataStream &str)const = 0;
	virtual void deserialize(QDataStream &str) = 0;
};

inline QDataStream& operator << (QDataStream &str, const serializable &rhs){
	rhs.serialize(str);
	return str;
}

inline QDataStream& operator >> (QDataStream &str, serializable &rhs){
	rhs.deserialize(str);
	return str;
}
