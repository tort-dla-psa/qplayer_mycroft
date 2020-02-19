#pragma once
#include <QVector>
#include <QString>
#include "serializable.h"

enum class magic_bytes_cmd{
	play_byte = (qint8)0,
	pause_byte,
	rewind_byte,

	list_dir_byte,
};

class command:public serializable{
public:
};

class media_command:public command{
public:
};

class play:public media_command{
public:
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_cmd::play_byte;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte();
	}
	void deserialize(QDataStream &str)override{}
};

class pause:public media_command{
public:
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_cmd::pause_byte;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte();
	}
	void deserialize(QDataStream &str)override{}
};

class rewind:public media_command{
	qint32 delta;
public:
	int get_delta()const{
		return delta;
	}
	void set_delta(int delta){
		this->delta = delta;
	}
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_cmd::pause_byte;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte() << delta;
	}
	void deserialize(QDataStream &str)override{
		str >> delta;
	}
};

class file_command:public command{};

class list:public file_command{
public:
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_cmd::list_dir_byte;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte();
	}
	void deserialize(QDataStream &str)override{}
};
