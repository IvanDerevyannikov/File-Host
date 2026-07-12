import componentRegister from "./component-register"

class ComponentManager{
    constructor(eventBus, registers, componentLoader){
        this.eventBus = eventBus;
        this.registers = registers;
        this.componentLoader
        this.compReg = new ComponentRegister();
    }

    async init(){
        const items = this.registers.map((item, index)=>{
            return new item.className(this.eventBus, this.componentLoader);
        })
        for (const item of items) {
            await item.init();
        }
    }
}