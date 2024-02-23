#include "FactoryMode.h"

#include <spdlog/spdlog.h>

#include <iostream>

int main(void) {
    spdlog::info("Hello World");

#ifdef SIMPLE_FACTORY
    Shoes *nike =
        ShoesProductFactory::GetInstance().GetMyProduct(ProductType::Nike);
    if (nike != nullptr) {
        nike->ShowMe();
        delete nike;
        nike = nullptr;
    }
    spdlog::info(
        "--------------------------------------------------------------");
    Shoes *nb =
        ShoesProductFactory::GetInstance().GetMyProduct(ProductType::NB);
    if (nb != nullptr) {
        nb->ShowMe();
        delete nb;
        nb = nullptr;
    }
#endif

#ifdef ABSTRACT_FACTORY
    Factory *nike_factory = new NikeProducer();

    Shoes *nike_shoes = nike_factory->CreatShoes();
    if (nike_shoes != nullptr) {
        nike_shoes->ShowMe();
        delete nike_shoes;
        nike_shoes = nullptr;
    }

    Clothe *nike_clothe = nike_factory->CreatClothe();
    if (nike_clothe != nullptr) {
        nike_clothe->ShowMe();
        delete nike_clothe;
        nike_clothe = nullptr;
    }

    delete nike_factory;
#endif

#ifdef TEMPLETE_ABSTRACT_FACTORY
    RealFactory<Shoes, NikeShoes> NikeShoesProduct;
    Shoes *nike_shoes_product = NikeShoesProduct.GetRealProductFromFactory();
    if (nike_shoes_product != nullptr) {
        nike_shoes_product->ShowMe();
        delete nike_shoes_product;
        nike_shoes_product = nullptr;
    }
#endif

#ifdef TEMPLETE_ABSTRACT_FACTORY_BETTER
    // 注册产品到工厂
    RealProductRegist<Shoes, NikeShoes> nike_shoes("NikeShoes");

    // 从工厂中获取产品
    Shoes *nike_shoe =
        ProductFactory<Shoes>::GetInstance().GetProduct("NikeShoes");
    if (nike_shoe != nullptr) {
        nike_shoe->ShowMe();
        delete nike_shoe;
        nike_shoe = nullptr;
    }

#endif

    return 0;
}