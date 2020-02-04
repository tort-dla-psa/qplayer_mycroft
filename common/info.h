#pragma once
#include <QString>
#include "serializable.h"

enum class magic_bytes_info{
	duration = (char)(1<<7), //most negative byte
	progress,
	artist,
	album,
	song,
};

class info:public serializable{
public:
};

class progress:public info{
	int m_progress;
public:
	int get_progress(){
		return m_progress;
	}
	void set_progress(int progress){
		this->m_progress = progress;
	}
	QByteArray serialize()const override{
		QByteArray data(get_length(), '0');
		data[0] = (char)magic_bytes_info::progress;
		memcpy(data.data()+1, &m_progress, 4);
		return data;
	}
	void deserialize(QByteArray data)override{
		data.remove(0,1);
		this->m_progress = data.toInt();
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_info::progress;
	}
	int get_length()const override{
		return 5;
	}
};

class artist:public info{
	QString m_artist;
public:
	QString get_name(){
		return this->m_artist;
	}
	void set_name(QString artist){
		this->m_artist = artist;
	}
	QByteArray serialize()const override{
		QByteArray data(get_length(), '0');
		data[0] = (char)magic_bytes_info::artist;
		memcpy(data.data()+1, m_artist.data(), m_artist.size());
		return data;
	}
	void deserialize(QByteArray data)override{
		data.remove(0, 1);
		this->m_artist = data;
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_info::progress;
	}
	int get_length()const override{
		return m_artist.length()+1;
	}
};
