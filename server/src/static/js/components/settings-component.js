import BaseComponent from "./base-component.js";

export default class SettingsComponent extends BaseComponent{
    
    constructor(options = {}) {
        super("settings-window", options);
        this.templatePath = "settings";
        this.cssRef = "/css/settings.css";
    }

    async init() {
        await super.init();
        this.applySettings();
    }

    subscribeToEvents(){

        const closeButton = document.getElementById("setting-close")
        this.bindEvent(closeButton, 'click', (e)=>this.closeSettings(e));

        const saveButton = document.getElementById("settings-save");
        this.bindEvent(saveButton, 'click', ()=>{this.saveSettings()});

    }

    closeSettings(event){

        const settingsContainer = document.getElementById("settings-overlay");
        settingsContainer.style.display="none";

    }

    saveSettings(){
        try{
            const settings ={
                language: document.querySelector('input[name="language"]:checked')?.value || 'en',
                theme: document.querySelector('input[name="theme"]:checked')?.value || 'light',
                compactnessMode: document.getElementById('compactness-switch').checked,
                sound: document.getElementById('sound-switch').checked,
                timeFormat: document.querySelector('select[name="time_format"]').value
            }

            localStorage.setItem('userSettings', JSON.stringify(settings));

            const settingsContainer = document.getElementById("settings-overlay");
            settingsContainer.style.display="none";
            const message = this.loader.getCompText("toast.settings-save");
            this.toastManager.success(message);
        } catch{
            const message = this.loader.getCompText("toast.settings-warning");
            this.toastManager.warning(message);
        }
    }
    
    applySettings() {

        const savedSettings = localStorage.getItem('userSettings');
        if(savedSettings){
            try {
                const settings = JSON.parse(savedSettings);

                const languageRadio = document.querySelector(`input[name="language"][value="${settings.language}"]`);
                if(languageRadio) {
                    languageRadio.checked = true;
                }

                const themeRadio = document.querySelector(`input[name="theme"][value="${settings.theme}"]`);
                if(themeRadio){
                    themeRadio.checked = true;
                }

                const compactnesModeChecker = document.getElementById('compactness-switch');
                if(compactnesModeChecker){
                    compactnesModeChecker.checked = settings.compactnessMode;
                }

                const soundChecker = document.getElementById('sound-switch');
                if(soundChecker) {
                    soundChecker.checked = settings.sound;
                }

                const timeFormatForm = document.querySelector('select[name="time_format"]')
                if(timeFormatForm) {
                    timeFormatForm.value = settings.timeFormat;
                }
            } catch {
                console.error("Fail to apply settings");
            }
        }

    }
}