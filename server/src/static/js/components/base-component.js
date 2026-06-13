export default class BaseComponent{
    constructor(containerId,eventBus){
        this.containerId = containerId;
        this.container = document.getElementById(containerId);
        this.eventBus = eventBus;
        this.listeners = [];
        this.isDestroyed = false;
    }

    listen(event,callback){
        const unListener = this.eventBus.on(event, callback, this);
        this.listeners.push(unListener);
    }

    $(selector){
        return document.querySelector(selector);
    }

    show(){
        this.container.style.hide = false;
    }

    hide(){
        this.container.style.hide = true;
    }

    destroy(){
        this.listeners.forEach(listener=> listener());
        this.listeners = [];
        this.container.innerHTML="";
        this.isDestroyed = true;
    }
}