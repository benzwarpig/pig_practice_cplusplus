#ifndef __FACTORYMODE_H
#define __FACTORYMODE_H

#include <iostream>
#include <map>

// #define SIMPLE_FACTORY
// #define ABSTRACT_FACTORY
// #define TEMPLETE_ABSTRACT_FACTORY
#define TEMPLETE_ABSTRACT_FACTORY_BETTER
/********************************************************
******************** 设计模式之工厂模式 ********************
********************************************************/

#ifdef SIMPLE_FACTORY
/********************************************************
简单工厂模式
    简单工厂模式的结构组成
        1.工厂类：工厂模式的核心类，会定义一个用于创建制定的具体实例对象的接口
        2.抽象产品类：是具体产品类的继承的父类或实现的接口
            3.具体产品累：工厂类所创建的对象就是此具体产品实例
        简单工厂模式特点
            工厂类封装了创建具体产品对象的函数
        缺点
            扩展性非常差，新增产品的时候，需要去修改工厂类
********************************************************/
enum class ProductType {
    Nike = 0,
    NB = 1,
    AD = 2,
};

class Shoes {
public:
    virtual void ShowMe() = 0;
    virtual ~Shoes(){};
};

class Nike : public Shoes {
public:
    void ShowMe() { std::cout << "i am Nike" << std::endl; }

    Nike() { std::cout << "Nike Create By Factory" << std::endl; };
    ~Nike() { std::cout << "Nike Destory By Factory" << std::endl; };
};

class AD : public Shoes {
public:
    void ShowMe() { std::cout << "i am AD" << std::endl; }

    AD() { std::cout << "AD Create By Factory" << std::endl; };
    ~AD() { std::cout << "AD Destory By Factory" << std::endl; };
};

class NB : public Shoes {
public:
    void ShowMe() { std::cout << "i am NB" << std::endl; }

    NB() { std::cout << "NB Create By Factory" << std::endl; };
    ~NB() { std::cout << "NB Destory By Factory" << std::endl; };
};

class ShoesProductFactory {
public:
    static ShoesProductFactory& GetInstance() {
        static ShoesProductFactory instance;
        return instance;
    }

public:
    Shoes* GetMyProduct(ProductType type) {
        switch (type) {
            case ProductType::Nike: {
                return new Nike();
            } break;

            case ProductType::NB: {
                return new NB();
            } break;

            case ProductType::AD: {
                return new AD();
            } break;
        }
    }

    ShoesProductFactory() {
        std::cout << "Shoes Factory Open !!!!" << std::endl;
    };
    ~ShoesProductFactory() {
        std::cout << "Shoes Factory Close !!!!" << std::endl;
    };
};
#endif

#ifdef ABSTRACT_FACTORY
/********************************************************
抽象工厂模式
    抽象工厂模式的结构组成
        1.抽象工厂类：工厂方法模式的核心类，提供创建具体产品的接口，由具体工厂类实现。
        2.具体工厂类：继承于抽象工厂，实现创建对应具体产品对象的方式。
        3.抽象产品类：它是具体产品继承的父类（基类）。
        4.具体产品类：具体工厂所创建的对象，就是此类。
    特点
        提供一个接口，可以创建多个产品族中的产品对象。如创建耐克工厂，则可以创建耐克鞋子产品、衣服产品、裤子产品等。
    缺点
        同工厂方法模式一样，新增产品时，都需要增加一个对应的产品的具体工厂类。
********************************************************/
class Shoes {
public:
    virtual void ShowMe() = 0;

    Shoes(){};
    virtual ~Shoes(){};
};

class Clothe {
public:
    virtual void ShowMe() = 0;

    Clothe(){};
    virtual ~Clothe(){};
};

class NikeShoes : public Shoes {
public:
    void ShowMe() { std::cout << "i am Nike Shoes" << std::endl; }

    NikeShoes() { std::cout << "NikeShoes Create !!!!" << std::endl; };
    ~NikeShoes() { std::cout << "NikeShoes Destory !!!!" << std::endl; };
};

class NikeClothe : public Clothe {
public:
    void ShowMe() { std::cout << "i am Nike Clothe" << std::endl; }

    NikeClothe() { std::cout << "NikeClothe Create !!!!" << std::endl; };
    ~NikeClothe() { std::cout << "NikeClothe Destory !!!!" << std::endl; };
};

class Factory {
public:
    virtual Shoes* CreatShoes() = 0;
    virtual Clothe* CreatClothe() = 0;

public:
    Factory(){};
    virtual ~Factory(){};
};

class NikeProducer : public Factory {
public:
    Shoes* CreatShoes() { return new NikeShoes(); }

    Clothe* CreatClothe() { return new NikeClothe(); }

public:
    NikeProducer() { std::cout << "NikeProducer Create" << std::endl; };
    ~NikeProducer() { std::cout << "NikeProducer Destory" << std::endl; };
};
#endif

#ifdef TEMPLETE_ABSTRACT_FACTORY
class Shoes {
public:
    virtual void ShowMe() = 0;

    Shoes(){};
    virtual ~Shoes(){};
};

class Clothe {
public:
    virtual void ShowMe() = 0;

    Clothe(){};
    virtual ~Clothe(){};
};

class NikeShoes : public Shoes {
public:
    void ShowMe() { std::cout << "i am Nike Shoes" << std::endl; }

    NikeShoes() { std::cout << "NikeShoes Create !!!!" << std::endl; };
    ~NikeShoes() { std::cout << "NikeShoes Destory !!!!" << std::endl; };
};

class NikeClothe : public Clothe {
public:
    void ShowMe() { std::cout << "i am Nike Clothe" << std::endl; }

    NikeClothe() { std::cout << "NikeClothe Create !!!!" << std::endl; };
    ~NikeClothe() { std::cout << "NikeClothe Destory !!!!" << std::endl; };
};

template <class AbstractType>
class AbstractFactory {
public:
    virtual AbstractType* GetRealProductFromFactory() = 0;

    AbstractFactory() {
        std::cout << "AbstractFactory Create !!!!" << std::endl;
    };
    virtual ~AbstractFactory() {
        std::cout << "AbstractFactory Destory !!!!" << std::endl;
    };
};

template <class AbstractType, class RealType>
class RealFactory {
public:
    AbstractType* GetRealProductFromFactory() { return new RealType(); }

    RealFactory() { std::cout << "RealFactory Create !!!!" << std::endl; };
    ~RealFactory() { std::cout << "RealFactory Destory !!!!" << std::endl; };
};
#endif

#ifdef TEMPLETE_ABSTRACT_FACTORY_BETTER

class Shoes {
public:
    virtual void ShowMe() = 0;

    Shoes(){};
    virtual ~Shoes(){};
};

class Clothe {
public:
    virtual void ShowMe() = 0;

    Clothe(){};
    virtual ~Clothe(){};
};

class NikeShoes : public Shoes {
public:
    void ShowMe() { std::cout << "i am Nike Shoes" << std::endl; }

    NikeShoes() { std::cout << "NikeShoes Create !!!!" << std::endl; };
    ~NikeShoes() { std::cout << "NikeShoes Destory !!!!" << std::endl; };
};

class NikeClothe : public Clothe {
public:
    void ShowMe() { std::cout << "i am Nike Clothe" << std::endl; }

    NikeClothe() { std::cout << "NikeClothe Create !!!!" << std::endl; };
    ~NikeClothe() { std::cout << "NikeClothe Destory !!!!" << std::endl; };
};

// 注册产品接口类
template <class ProductType>
class AbstractProductRegist {
public:
    virtual ProductType* GetRealProduct() = 0;

protected:
    AbstractProductRegist(){};
    virtual ~AbstractProductRegist(){};

private:
    AbstractProductRegist(const AbstractProductRegist&);
    const AbstractProductRegist& operator=(const AbstractProductRegist&);
};

// 工厂模板类
template <class ProductType>
class ProductFactory {
public:
    static ProductFactory& GetInstance() {
        static ProductFactory instance;
        return instance;
    }

    void RegistProductToFactory(AbstractProductRegist<ProductType>* tmp,
                                std::string name) {
        my_product_part[name] = tmp;
    }

    ProductType* GetProduct(std::string name) {
        // 从map中找到已经注册过的产品，并返回产品对象
        if (my_product_part.find(name) != my_product_part.end()) {
            return my_product_part[name]->GetRealProduct();
        }

        std::cout << "No Product Found For " << name << std::endl;

        return NULL;
    }

private:
    ProductFactory(){};
    ~ProductFactory(){};

    ProductFactory(const ProductFactory&);
    const ProductFactory& operator=(const ProductFactory&);

    std::map<std::string, AbstractProductRegist<ProductType>*> my_product_part;
};

// 用于创建具体的产品和在工厂中注册
template <class ProductType, class Product>
class RealProductRegist : public AbstractProductRegist<ProductType> {
public:
    explicit RealProductRegist(std::string name) {
        // 通过工厂单例，把产品注册到工厂
        ProductFactory<ProductType>::GetInstance().RegistProductToFactory(this,
                                                                          name);
    }

    ProductType* GetRealProduct() { return new Product(); }
};

#endif

#endif