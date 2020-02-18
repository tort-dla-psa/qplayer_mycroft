#pragma once
#include <QString>
#include "serializable.h"
#include "utils.h"

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

class dir:public info{
	QVector<QString> dirs;
	QVector<QString> files;
public:
	QByteArray serialize()const override{
		//TODO:utf8
		QByteArray data(get_length(), '0');
		char* data_ptr = data.data();
		data[0] = (char)magic_bytes_info::dir;
		auto dirs_bytes = utils::serialize(dirs);
		auto files_bytes = utils::serialize(files);
		data_ptr++;
		memcpy(data_ptr, dirs_bytes.data(), dirs_bytes.size());
		data_ptr += dirs_bytes.size();
		memcpy(data_ptr, files_bytes.data(), files_bytes.size());
		return data;
	}
	void deserialize(QByteArray data)override{
		data.remove(0, 1); //delete magic byte
		char* data_ptr = data.data();

		auto tmp_dirs_pair = utils::deserialize_str_vec(data.data(), data.size());
		this->dirs = tmp_dirs_pair.first;
		int bytes_processed = tmp_dirs_pair.second;
		data_ptr += bytes_processed;

		auto tmp_files_pair = utils::deserialize_str_vec(data_ptr, data.size()-bytes_processed);
		this->files = tmp_files_pair.first;
	}
	char get_magic_byte()const override{
		return (char)magic_bytes_info::dir;
	}
	int get_length()const override{
		int len = 1; //to store magic byte
		len += sizeof(int); //to store dirs count
		len += dirs.size()*sizeof(int); //to store dirs names length
		for(auto &d:dirs){
			len += d.size();
		}
		len += sizeof(int); //to store files count
		len += files.size()*sizeof(int);
		for(auto &f:files){
			len += f.size();
		}
		return len;
	}
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
};