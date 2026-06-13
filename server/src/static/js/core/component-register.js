export default class ComponentRegister{
    constuctor(){
        this.registeredComponents = new Set();
    }

    register(name, componentClassName){
        this.registeredComponents.set(name,componentClassName);
    }

    get(name){
        return this.registeredComponents.get(name);
    }

    has(name){
        return this.registeredComponents.has(name);
    }
}