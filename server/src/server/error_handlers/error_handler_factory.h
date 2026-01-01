#ifndef ERROR_HANDLER_FACTORY_HEAD
#define ERROR_HANDLER_FACTORY_HEAD

#include "error_handler.h"
#include <memory>
#include <map>

class AbstractErrorHandlerCreator{

public:
    virtual ~AbstractErrorHandlerCreator() = default;
    virtual void errorCreator(){}

    virtual std::unique_ptr<IErrorHandler> create() const = 0;
};


template<typename ErrorHandler>
class ErrorHandlerCreator: public AbstractErrorHandlerCreator{
public:

    std::unique_ptr<IErrorHandler> create() const override{
        return std::make_unique<ErrorHandler>();
    }

};

class ErrorHandlerFactory{
public:
    enum class ErrorHandlerType{
        JsonErrorType,
        ImageErrorType,
        HtmlErrorType,
        DefaultType,
    };

public:
    std::unique_ptr<IErrorHandler> createErrorHandler(ErrorHandlerFactory::ErrorHandlerType type){
        
        if(creatorMap_.find(type)!=creatorMap_.end()){
            return creatorMap_[type]->create();
        }
        return creatorMap_[ErrorHandlerType::DefaultType]->create();
    }
    
    template <typename ErrorHandler>
    void addErrorHandler(ErrorHandlerFactory::ErrorHandlerType type){
        //auto creatorMapIt = creatorMap_.find(type);

        if(creatorMap_.find(type)==creatorMap_.end()){
            creatorMap_[type] = std::make_unique<ErrorHandlerCreator<ErrorHandler>>();
        }
    }

    void initialize(){
        addErrorHandler<JsonErrorHandler>(ErrorHandlerType::JsonErrorType);
        addErrorHandler<HtmlErrorHandler>(ErrorHandlerType::HtmlErrorType);
        addErrorHandler<ImageErrorHandler>(ErrorHandlerType::ImageErrorType);
        addErrorHandler<DefaultErrorHandler>(ErrorHandlerType::DefaultType);
    }
private:

    std::map<ErrorHandlerType, std::unique_ptr<AbstractErrorHandlerCreator>> creatorMap_;
};


#endif