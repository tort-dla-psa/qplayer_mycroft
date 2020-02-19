#include <QThread>
#include <QDebug>
#include <QException>
#include <QMediaPlaylist>
#include "play_worker.h"
#include "info.h"

play_worker::play_worker(QObject *parent)
	:QObject(parent)
{
	m_progress = 0;
	update_usleep = 3000;
	paused = false;
}

void play_worker::open_dir(QString path){
	this->dir = QDir(path);
}
void play_worker::start(){
	if(!dir.exists()){
		return;
		//TODO:throw exception
	}
	QStringList filter;
	filter << "*.mp3" << "*.MP3";
	filter << "*.flac" << "*.FLAC";
	auto dir_items = dir.entryList(filter, QDir::Files|QDir::Readable);

	QMediaPlaylist *playlist = new QMediaPlaylist();
	for(auto d:dir_items){
		auto cast = QMediaContent(QUrl::fromLocalFile(dir.path()+"/"+d));
		playlist->addMedia(cast);
	}
	playlist->setPlaybackMode(QMediaPlaylist::Loop);

	this->music = new QMediaPlayer();
	music->setPlaylist(playlist);

	//emit progress
	//connect(music, &QMediaPlayer::positionChanged,
	//		this, &play_worker::progress);

	//auto overloaded_func = QOverload<const QString &, const QVariant &>::of(&QMediaPlayer::metaDataChanged);
	//connect(music, overloaded_func,
	//		this, &play_worker::on_metadata_change);
	music->play();
	/*
	while(true){
		while(paused){
			QThread::currentThread()->msleep(update_usleep);
		}
		m_progress++;
		emit progress(m_progress);
		print(QString::number(m_progress));
		QThread::currentThread()->msleep(update_usleep);
	}*/
}

void play_worker::pause(){
	print("got paused cmd, now:"+QString(!paused?"true":"false"));
	paused = !paused;
	if(!paused){
		music->pause();
	}else{
		music->play();
	}
}
void play_worker::rewind(int delta){
	print("got move progress cmd:"+QString::number(delta));
	music->setPosition(music->position()+delta);
}
void play_worker::process_cmd(QSharedPointer<command> data){
	{
		auto pause_cmd = data.dynamicCast<class pause>();
		if(pause_cmd){
			pause();
			return;
		}
	}{
		auto rewind_cmd = data.dynamicCast<class rewind>();
		if(rewind_cmd){
			rewind(rewind_cmd->get_delta());
			return;
		}
	}
}
void play_worker::on_metadata_change(const QString &key, const QVariant &value){
	if(key == "Artist"){
		auto artists = value.toStringList();
		artist a;
		a.set_name(artists.front());
		emit artist_changed(a);
	}
}
