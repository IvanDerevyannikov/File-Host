import BaseComponent from "./base-component.js";
import SettingsComponent from "./settings-component.js";

export default class HeaderComponent extends BaseComponent{
    
    constructor(options ={}){
        super("header-main", options);

        this.templatePath = "header";
    }

    subscribeToEvents(){
        this.listen('settings:opened', ()=>{
            console.log('[Header] settings opened');
        })
        this.listen('settings:closed', ()=>{
            console.log('[Header] settings closed');
        })
        const el = document.getElementById("settings-container");
        this.bindEvent(el, 'click', async (event)=>{
            const settingsComp = new SettingsComponent({eventBus: this.eventBus, loader: this.loader, toastManager: this.toastManager});
            await settingsComp.init();
        })
    }

    // async init(){
    //     await this.loader.loadComponent(this.templatePath, this.containerId);
    // }
}