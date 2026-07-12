export default class BaseComponent{
    constructor(containerId, options = {}){
        this.containerId = containerId;
        this.container = document.getElementById(containerId);
        this.eventBus = options.eventBus;
        this.loader = options.loader;
        this.toastManager = options.toastManager;
        this.listeners = [];
        this.initialized = false;
        this.isDestroyed = false;
    }

    async init(){
        if(this.initialized) return;

        if(this.loader && this.templatePath){
            await this.loader.loadComponent(this.templatePath, this.containerId, this.cssRef);
        }

        this.subscribeToEvents();
        this.initialized = true;
    }

    subscribeToEvents(){

    }
    
    on(selector, event, handler){
        this.eventBus.on(selector, event, handler);
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

    bindEvent(element, event, handler){
        const el = typeof element==="string" ? this.$(element) : element;
        if(!el){
            console.error(event, " could not load element");
        }
        // const boundHandler = handler.bind(this);
        el.addEventListener(event, handler);
        // this.listeners.push({element: el, event: event, handler: boundHandler});_
    }

    bindEvents(){
        this.listeners.forEach((element, event, handler)=>{
            element.removeEventListener(event, handler);
        })
    }

    destroy(){
        this.listeners.forEach(listener=> listener());
        this.listeners = [];
        this.container.innerHTML="";
        this.isDestroyed = true;
    }
}