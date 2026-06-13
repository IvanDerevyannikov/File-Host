export default class DomTranslator{

    constructor(localeManager){
        this.localeManager=localeManager;
    }

    applyLocales(containerId){
        const container = document.getElementById(containerId);
        
        if(!container){
            console.error("container is not existed on the page!");
            return;
        }

        const elements = container.querySelectorAll("data-i18n");
        elements.forEach((element)=>{
            this.applyElementLocales(element);
        })
    }

    applyElementLocales(element){
        const key = element.getAttibute("data-i18n");
        const text = this.localeManager.getText(key);
        
        if(text===null){
            console.error(`cannot load ${key} text element for ${this.localeManager.lang} language`);
            return;
        }
        applyTextForElement(element, text);
    }

    applyTextForElement(element,text){
        if(element.tagName==="INPUT" || element.tagName==="TEXTAREA"){
            if(element.type==="placeholder" || element.hasAttribute("placeholder")){
                element.placeholder = text;
            } else {
                element.value = text;
            }
        } else {
            element.innerHTML = text;
        }
    }

}