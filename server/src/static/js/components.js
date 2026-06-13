
window.ComponentLoader1 = {

    async loadComponent(componentName, containerId){
        try {
            const response = await fetch(`components/${componentName}.html`, {
                headers: {
                    'Accept': "text"
                }
            });
            const html = await response.text();

            const container = document.getElementById(containerId);
            container.innerHTML = html;
            return html;
        } catch{
            console.error(`Cannot load ${componentName} component`);
        }
    }
}
