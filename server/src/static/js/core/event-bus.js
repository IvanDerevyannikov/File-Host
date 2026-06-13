export default class EventBus{
    constructor(){
        this.listeners = new Map();
    }

    on(event, callback, context){
        if(!this.listeners.has(event)){
            this.listeners.set(event,[]);
        }
        this.listeners.get(event).push(callback, context);
        return ()=>this.off(event,callback);
    }

    off(event,callback){
        if(!this.listeners.has(event)) return;
        const filtered  =  this.listeners.get(event)
            .filter(listener=> listener.callback!=callback);
        return this.listeners.set(event,filtered);
    }

    emit(event, data={}){
        if(!this.listeners.has(event)) return;
        this.listeners.get(event).forEach(listener => {
            listener.callback.call(listener.context, data)
        });
    }
}