#pragma once
#include "serializable.h"

enum class magic_bytes_info{
	duration = '1',
	progress,
	author,
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
	QByteArray serialize()override{
		QByteArray data(5, '0');
		data[0] = (char)magic_bytes_info::progress;
		memcpy(data.data()+1, &m_progress, 4);
		return data;
	}
	void deserialize(QByteArray data)override{
		data.remove(0,1);
		this->m_progress = data.toInt();
	}
};
