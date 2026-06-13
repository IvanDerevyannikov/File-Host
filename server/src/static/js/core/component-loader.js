export default class ComponentLoader {
    constructor(domTranslator){
        this.domTranslator = domTranslator;
        this.loadComponents = new Set();
    }
    
    async loadHTML(componentName){
        const response = await fetch(`components/${componentName}.html`,{
            headers: {
                'Accept': "text"
            }
        })
        const text = await response.text();
        return text;
    }

    loadCSS(cssRef){
        return new Promise((resolve, reject)=>{
            const cssLink = document.createElement("link");
            cssLink.rel = "stylesheet"
            cssLink.type = "text/css"
            cssLink.href = cssRef;

            cssLink.onload = () => resolve();
            cssLink.onerror = () => reject();
            
            document.head.appendChild(cssLink);
        })
    }

    async loadComponent(componentName, containerId,cssRef){
        await this.loadCSS(cssRef);
        const htmlText = await this.loadHTML(componentName);
        const container = document.getElementById(containerId);
        container.innerHTML = htmlText;

        domTranslator.applyLocales(containerId);
        this.loadComponents.add(containerId);
    }

    anmountComponent(containerId){
        if(!this.loadComponents.has(containerId)){
            console.warn(`Container ${containerId} cannot be inserted in page`);
            return;
        }
        this.loadComponents.delete(containerId);
        const container = document.getElementsById(containerId);
        container.innerHTML = "";
    }
} 