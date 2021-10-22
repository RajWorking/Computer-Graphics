import * as THREE from 'three';
import { scene, camera } from './index';
import GameObject from './object';
import * as CONFIG from './config';

export default class Star extends GameObject {
    constructor(position, body = '/models/star.gltf', scale = 0.2) {
        super(position, body, scale);
    }

    // move(target) {
    //     var toward = new THREE.Vector3();

    //     if (this.Mesh) {
    //         var my_pos = this.Mesh.position.clone();
    //         my_pos.multiplyScalar(-1);
    //         toward.addVectors(my_pos, target);
    //         toward.divideScalar(toward.length());
    //         toward.multiplyScalar(CONFIG.ENEMY_SPEED);
    //         toward.y += CONFIG.CAM_SPEED;

    //         this.Mesh.lookAt(target);
    //         this.Mesh.position.add(toward);
    //     }
    // }

    rotate() {
        if (this.Mesh) {
            this.Mesh.rotation.y -= 0.02;
        }
    }
}
