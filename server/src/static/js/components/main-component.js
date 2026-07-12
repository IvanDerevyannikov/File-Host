import BaseComponent from "./base-component.js";

export default class MainComponent extends BaseComponent{

    constructor(options={}) {
        super("main-content", options);

        this.templatePath = "main_content";
    }
}