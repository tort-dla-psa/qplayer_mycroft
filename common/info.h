#pragma once
#include <QString>
#include "serializable.h"

enum class magic_bytes_info{
	duration = (char)(1<<7), //most negative byte
	progress,
	artist,
	album,
	song,

	dir
};

class info:public serializable{
public:
};

class progress:public info{
	qint32 m_progress;
public:
	int get_progress(){
		return m_progress;
	}
	void set_progress(int progress){
		this->m_progress = progress;
	}
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_info::progress;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte() << m_progress;
	}
	void deserialize(QDataStream &str)override{
		str >> m_progress;
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
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_info::progress;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte() << m_artist;
	}
	void deserialize(QDataStream &str)override{
		str >> m_artist;
	}
};

class dir:public info{
	QVector<QString> dirs;
	QVector<QString> files;
public:
	void set_dirs(const QVector<QString> &dirs){
		this->dirs = dirs;
	}
	void set_files(const QVector<QString> &files){
		this->files = files;
	}
	QVector<QString> get_dirs(){
		return dirs;
	}
	const QVector<QString>& get_dirs()const{
		return dirs;
	}
	QVector<QString> get_files(){
		return files;
	}
	const QVector<QString>& get_files()const{
		return files;
	}
	qint8 get_magic_byte()const override{
		return (qint8)magic_bytes_info::dir;
	}
	void serialize(QDataStream &str)const override{
		str << get_magic_byte() << dirs << files;
	}
	void deserialize(QDataStream &str)override{
		str >> dirs;
		str >> files;
	}
};
