import * as THREE from 'three';
import { scene, camera } from './index';
import GameObject from './object';
import * as CONFIG from './config';

export default class Enemy extends GameObject {
    constructor(position, body = '/models/ufo.gltf', scale = 0.3) {
        super(position, body, scale);
        this.velocity = CONFIG.ENEMY_SPEED;
        this.direction = 2 * Math.floor(Math.random() + 1) - 1;
    }

    move(target) {
        var toward = new THREE.Vector3();

        if (this.Mesh) {
            var my_pos = this.Mesh.position.clone();
            my_pos.multiplyScalar(-1);
            toward.addVectors(my_pos, target);
            toward.divideScalar(toward.length());
            toward.multiplyScalar(CONFIG.ENEMY_SPEED);
            toward.y += CONFIG.CAM_SPEED;

            this.Mesh.lookAt(target);
            this.Mesh.position.add(toward);
        }
    }

    // rotate() {
    //     if (this.Mesh) {
    //         this.Mesh.rotation.y -= 0.02;
    //     }
    // }
}
