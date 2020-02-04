#pragma once
#include <QByteArray>

class serializable{
public:
	virtual char get_magic_byte()const=0;
	virtual QByteArray serialize()const=0;
	virtual void deserialize(QByteArray data)=0;
	virtual int get_length()const=0;
};
