import LocalesProvider from './i18n.js';
import LocaleManager from './locale-manager.js';
import DomTranslator from './dom-translator.js';
import ComponentLoader from './core/component-loader.js';
import HeaderComponent from './components/header-component.js';
import EventBus from './core/event-bus.js'
import MainComponent from './components/main-component.js';
import SettingsComponent from './components/settings-component.js'
import FooterComponent from './components/footer-component.js'
import ToastManager from './core/toast-manager.js';

document.addEventListener('DOMContentLoaded', async function () {
    console.log("Dom is reqdy... start main function")

    // ComponentLoader1.loadComponent("header", "header-main").then(() => {
    //     const settingButton = document.getElementById("settings-container");
    //     settingButton.addEventListener('click', () => {
    //         loadCSSWithPromise().then(()=>{
    //             ComponentLoader1.loadComponent("settings", "settings-window");
    //             console.log(12);
    //         })
    //     })
    // });
    // ComponentLoader1.loadComponent("main_content", "main-content");

    // const locales = new LocalesProvider();
    // locales.loadLocale();
    // locales.applyLocale();

    const localeManager = new LocaleManager();
    await localeManager.loadLocale();
    const domTranslator = new DomTranslator(localeManager);
    const compLoader = new ComponentLoader(domTranslator);
    const compArr = ["header", "settings", "main_content"];

    compLoader.loadHTML("header").then((resolve, reject)=>{
        console.log("header loaded");
    });
    compLoader.loadHTML("settins").then((resolve, reject)=>{
        console.log("settings loaded");
    });
    compLoader.loadHTML("main_content").then((resolve, reject)=>{
        console.log("main_content loaded");
    });

    const eventBus = new EventBus();
    const toastManager = new ToastManager();
    const headerComp = new HeaderComponent( 
        {    
            eventBus: eventBus,
            loader: compLoader,
            toastManager: toastManager,
        });
    await headerComp.init();

    const mainComp = new MainComponent(
        {
            eventBus: eventBus,
            loader: compLoader,
            toastManager: toastManager,
        });
    await mainComp.init();

    const footerComp = new FooterComponent(
        {
            eventBus: eventBus,
            loader: compLoader,
            toastManager: toastManager,
        });
    await footerComp.init();
})