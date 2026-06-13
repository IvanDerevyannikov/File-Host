export default class LocaleManager {

    constructor(lang = "en") {
        this.lang = lang;
        this.translations = {};
        this.supportedLang = ["ru", "en"];
    }

    async loadLocale() {
        try {
            const response = await fetch(`/locales/${this.lang}`);

            if (!response.ok) {
                console.log("fail");
            }

            const jsonData = await response.json();
            this.translations[this.lang] = jsonData;

        } catch (error) {
            console.error("Error loading locale", error);

            if (this.lang !== "en") {
                this.lang = "en";
                await this.loadLocale();
            }

        }
    }

    getText(key) {
        if (!this.translations[this.lang]) {
            console.warn(`Translations for ${this.lang} not loaded`);
            return key;
        }

        return key.split(".").reduce((obj, keyPart) => {
            return obj && obj[keyPart] !== undefined ? obj[keyPart] : null;
        }, this.translations[this.lang]);
    }

    setLanguage(lang) {
        if (this.supportedLang.includes(lang)) {
            this.lang = lang;
            const success = this.loadLocale().then(() => { return true }).catch(() => { return false });
            return success;
        }
        return false;
    }
}