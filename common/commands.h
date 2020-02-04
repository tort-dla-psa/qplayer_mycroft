#pragma once
#include "serializable.h"
#include <QVector>
#include <QString>

enum class magic_bytes_cmd{
	play_byte = '1',
	pause_byte = '2',
	rewind_byte = '3'
};

class command:public serializable{
public:
};

class media_command:public command{
public:
};

class play:public media_command{
public:
	QByteArray serialize()const override{
		return "";
	}
	void deserialize(QByteArray data)override{
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_cmd::play_byte;
	}
	int get_length()const override{
		return 1;
	}
};

class pause:public media_command{
public:
	QByteArray serialize()const override{
		return "";
	}
	void deserialize(QByteArray data)override{
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_cmd::pause_byte;
	}
	int get_length()const override{
		return 1;
	}
};

class rewind:public media_command{
	int delta;
public:
	int get_delta()const{
		return delta;
	}
	void set_delta(int delta){
		this->delta = delta;
	}
	QByteArray serialize()const override{
		QByteArray bytes(get_length(), '0');
		char byte = (char)magic_bytes_cmd::rewind_byte;
		memcpy(bytes.data(), &byte, 1);
		memcpy(bytes.data()+1, &delta, 4);
		return bytes;
	}
	void deserialize(QByteArray data)override{
		data.remove(0, 1); //NOTE:remove magic byte
		delta = data.toInt();
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_cmd::pause_byte;
	}
	int get_length()const override{
		return 5;
	}
};

class file_command:public command{
};

class list:public file_command{
	QVector<QString> names;
	char delimeter='\0',
	m_magic_byte='1';
public:
	QByteArray serialize()const{
		QByteArray result;
		for(const auto &name:names){
			QByteArray bytes = name.toUtf8();
			bytes.push_back(delimeter);
			result.insert(result.size(), bytes);
		}
		return result;
	}
	void deserialize(QByteArray data){
		int prev_index = 0;
		int index = data.indexOf(delimeter);
		while(index != -1){
			QString name;
			auto subvector = data.mid(prev_index, index);
			name.fromUtf8(subvector);
			names.push_back(name);
			data.remove(prev_index, subvector.size()+1);
			prev_index = index;
			index = data.indexOf(delimeter);
		}
	}
	char get_magic_byte()const{
		return m_magic_byte;
	}
};
