#pragma once
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QPair>

namespace utils{

//string
inline QByteArray serialize(const QString &data){
    int len = 0;
    len += sizeof(int);
    len += data.size();
    QByteArray bytes(len, ' ');
    int data_len = data.size();
    memcpy(bytes.data(), &data_len, sizeof(data_len));
    memcpy(bytes.data()+sizeof(int), data.data(), data_len);
    return bytes;
}
inline QPair<QString, int> deserialize_str(const char* ptr, int len){
    int str_len;
    memcpy(&str_len, ptr, sizeof(str_len));
    QString str(str_len, ' ');
    memcpy(str.data(), ptr+sizeof(str_len), str_len);
    return {str, str_len+(int)sizeof(str_len)};
}
inline QPair<QString, int> deserialize_str(const QByteArray &bytes){
    return deserialize_str(bytes.data(), bytes.size());
}

//vector of strin
inline QByteArray serialize(const QVector<QString> &data){
    int len = 0;
    len += sizeof(int); // to store list len;
    len += sizeof(int)*data.size(); //to store strings sizes
    for(const auto &str:data){
        len += str.size();
    }
    QByteArray bytes(len, ' ');
    auto data_ptr = bytes.data();

    int data_len = data.size();
    memcpy(data_ptr, &data_len, sizeof(data_len));
    data_ptr+= sizeof(data_len);

    for(const auto &str:data){
        auto str_bytes = serialize(str);
        memcpy(data_ptr, str_bytes.data(), str_bytes.size());
        data_ptr+=str_bytes.size();
    }
    return bytes;
}

inline QPair<QVector<QString>, int> deserialize_str_vec(const char* data_ptr, size_t size){
    QVector<QString> data;

    int len;
    memcpy(&len, data_ptr, sizeof(len)); //read vector size
    data_ptr += sizeof(len);
    int counter= sizeof(len);

    for(int i=0; i<len; i++){
        auto str_pair = deserialize_str(data_ptr, size-counter);
        data_ptr += str_pair.second;
        counter += str_pair.second;
        data.push_back(str_pair.first);
    }
    return {data, counter};
}
inline QPair<QVector<QString>, int> deserialize(const QByteArray &bytes){
    return deserialize_str_vec(bytes.data(), bytes.size());
}

}