<?php

// This file has been auto-generated by the Symfony Dependency Injection Component for internal use.

if (\class_exists(\ContainerW0ox38w\appProdProjectContainer::class, false)) {
    // no-op
} elseif (!include __DIR__.'/ContainerW0ox38w/appProdProjectContainer.php') {
    touch(__DIR__.'/ContainerW0ox38w.legacy');

    return;
}

if (!\class_exists(appProdProjectContainer::class, false)) {
    \class_alias(\ContainerW0ox38w\appProdProjectContainer::class, appProdProjectContainer::class, false);
}

return new \ContainerW0ox38w\appProdProjectContainer([
    'container.build_hash' => 'W0ox38w',
    'container.build_id' => '19ddfc08',
    'container.build_time' => 1637934611,
], __DIR__.\DIRECTORY_SEPARATOR.'ContainerW0ox38w');
