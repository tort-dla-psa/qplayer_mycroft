#pragma once
#include <QByteArray>
#include <QVector>
#include <QString>

class command{
public:
    virtual QByteArray serialize()const=0;
    virtual void deserialize(QByteArray data)=0;
    virtual char magic_byte()const=0;
};

class media_command:public command{
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
        for(size_t i=0; i<names.size(); i++){
            auto &name = names.at(i);
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
    char magic_byte()const{
        return m_magic_byte;
    }
};
