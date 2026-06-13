export default class BaseController {
    
    constructor(container, options={}){
        this.container=container;
        this.options=options;
        this.events = [];
    }

    bindEvent(element, event, handler){
        const container =  typeof element == "string" 
            ? document.querySelector(element) 
            : element;
        
        if(container) {
            container.addEventListener(event, handler);
            this.events.push(event);
        } else {
            console.warm("In container ", container.className, " cannot be bind event ", event);
        }
    }
    
    bindEvents(eventsMap){
        Object.entries(eventsMap).forEach(([selector, handlers]) => {
            Object.entries(handlers).forEach(([event,handler]) => {
                this.bindEvent(selector,event,handler);
            });
        })
    }

    $(selector){
        return this.container.querySelector(selector);
    }

    $$(selectors){
        return this.container.querySelectorAll(selectors);
    }

    destroy(){
        
    }

}