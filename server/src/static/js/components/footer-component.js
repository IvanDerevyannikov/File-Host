import BaseComponent from './base-component.js'

export default class FooterComponent extends BaseComponent {

    constructor(options = {}){
        super("footer-info", options);

        this.templatePath = "footer";
    }
}