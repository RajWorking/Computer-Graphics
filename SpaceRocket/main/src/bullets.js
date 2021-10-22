import * as THREE from 'three';
import { scene, camera } from './index';
import GameObject from './object';
import * as CONFIG from './config';

export default class Bullet extends GameObject {
    constructor(position, body = '/models/bullet.gltf', scale = 0.2) {
        super(position, body, scale);
        this.velocity = CONFIG.BULLET_SPEED;
    }

    move() {
        if (this.Mesh && this.Mesh.position) {
            this.Mesh.position.y += this.velocity;
        }
    }
}
