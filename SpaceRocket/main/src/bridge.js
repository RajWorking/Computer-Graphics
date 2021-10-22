import * as THREE from 'three';
import { scene, camera } from './index';
import GameObject from './object';

export default class Bridge extends GameObject {
    constructor(position, body = '/models/object.gltf', scale = 1) {
        super(position, body, scale);
    }
}
