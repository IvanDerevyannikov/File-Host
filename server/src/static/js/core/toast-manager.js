export default class ToastManager{
    constructor(){
        this.currentToast = null;
        this.loadCSS();
    }

    loadCSS(){
        if(document.querySelector('link[href="/css/toast.css"]')){
            return;
        }

        const link = document.createElement('link');
        link.rel = "stylesheet";
        link.href = "/css/toast.css";
        document.head.appendChild(link);

    }

    show(message, status, duration){
        
        if(this.currentToast){
            this.currentToast.remove();
            this.currentToast = null;
        }

        const toast = document.createElement('div');
        toast.className = `toast-notification toast-notification-${status}`;
        toast.textContent = message;
        this.currentToast = toast;
        
        document.body.appendChild(toast);
        
        requestAnimationFrame(() => {
            toast.classList.add('toast-notification-visible');
        });

        setTimeout(()=>{
            toast.classList.remove('toast-notification-visible');
            setTimeout(()=>{
                toast.remove();
            }, 300)
        }, duration);
    }

    success(message){
        this.show(message, 'success', 2000);
    }

    warning(message){
        this.show(message, 'warning', 3000);
    }

    error(message){
        this.show(message, 'error', 3000);
    }

}