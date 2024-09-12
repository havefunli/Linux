#pragma once 

// 该类为一个基类，删除拷贝构造函数，赋值运算符重载函数
// 继承该基类的子类的相关函数也会被删除
// 是因为该子类没有显示实现相关函数时，会生成一个默认的相关函数
// 该默认拷贝构造函数会调用基类的相关拷贝构造函数
// 但是该函数被删除，所以子类会因为无法调用而被隐式删除


class nocopy
{
public:
    nocopy(){};
    // 删除相关拷贝函数
    nocopy(const nocopy&) = delete;
    const nocopy& operator=(const nocopy&) = delete;
};