export class Store {
  constructor(initial = {}) {
    this.state = initial;
    this.subscribers = new Map();
  }

  get() {
    return this.state;
  }

  set(partial) {
    this.state = { ...this.state, ...partial };
    this._notify();
  }

  subscribe(key, cb) {
    if (!this.subscribers.has(key)) this.subscribers.set(key, []);
    this.subscribers.get(key).push(cb);
    return () => this.unsubscribe(key, cb);
  }

  unsubscribe(key, cb) {
    if (!this.subscribers.has(key)) return;
    const arr = this.subscribers.get(key).filter((f) => f !== cb);
    this.subscribers.set(key, arr);
  }

  _notify() {
    for (const [key, cbs] of this.subscribers.entries()) {
      const val = this.state[key];
      cbs.forEach((cb) => cb(val, this.state));
    }
  }
}