import LocalesProvider from './i18n.js';
import LocaleManager from './locale-manager.js';
import DomTranslator from './dom-translator.js';
import ComponentLoader from './component-loader.js';

document.addEventListener('DOMContentLoaded', function () {
    console.log("Dom is reqdy... start main function")

    ComponentLoader1.loadComponent("header", "header-main").then(() => {
        const settingButton = document.getElementById("settings-container");
        settingButton.addEventListener('click', () => {
            loadCSSWithPromise().then(()=>{
                ComponentLoader1.loadComponent("settings", "settings-window");
                console.log(12);
            })
        })
    });
    ComponentLoader1.loadComponent("main_content", "main-content");

    const locales = new LocalesProvider();
    locales.loadLocale();
    locales.applyLocale();

    const localeManager = new LocaleManager();
    const domTranslator = new DomTranslator(localeManager);
    const compLoader = new ComponentLoader(domTranslator);
})


function loadCSSWithPromise(){
    return new Promise((resolve, reject)=>{
        const cssLink = document.createElement("link");
        cssLink.rel = "stylesheet"
        cssLink.type = "text/css"
        cssLink.href = "/css/settings.css"

        cssLink.onload = () => {
            console.log("CSS loaded successfully");
            resolve();
        };
        
        cssLink.onerror = () => {
            console.error("Failed to load CSS");
            reject(new Error(`Failed to load`));
        };
        
        document.head.appendChild(cssLink);
    })
}