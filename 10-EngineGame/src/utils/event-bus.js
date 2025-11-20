export class EventBus {
  constructor() {
    this.listeners = new Map();
  }

  on(event, cb) {
    if (!this.listeners.has(event)) this.listeners.set(event, []);
    this.listeners.get(event).push(cb);
    return () => this.off(event, cb);
  }

  off(event, cb) {
    if (!this.listeners.has(event)) return;
    const arr = this.listeners.get(event).filter((f) => f !== cb);
    this.listeners.set(event, arr);
  }

  emit(event, payload) {
    (this.listeners.get(event) || []).slice().forEach((cb) => cb(payload));
  }
}
