export default class LocalesProvider {

    constructor(lang = "en") {
        this.lang = lang;
        this.translations = {};
        this.supportedLang = ["ru", "en"];
    }

    changeLang(lang) {
        this.lang = lang;
        this.loadLocal();
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

    async applyLocale() {
        if (!this.translations[this.lang]) {
            await this.loadLocale();
        }

        const elements = document.querySelectorAll('[data-i18n]')

        elements.forEach((element) => {
            const key = element.getAttribute("data-i18n");

            const value = this.getElementText(this.translations[this.lang], key);

            if (value === null) {
                console.error(`cannot load ${key} text element for ${this.lang} language`);
                return;
            }

            if (element.tagName === 'INPUT' || element.tagName === 'TEXTAREA') {
                if (element.type === 'placeholder' || element.hasAttribute('placeholder')) {
                    element.placeholder = value;
                } else {
                    element.value = value;
                }
            } else {
                element.innerHTML = value;
            }

        })
    }

    getElementText(translations, key) {
        return key.split(".").reduce((obj, keyPart) => {
            return obj && obj[keyPart] !== undefined ? obj[keyPart] : null;
        }, translations);
    }

}